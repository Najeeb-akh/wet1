#include "worldcup23a1.h"
#include "wet1util.h"
#include "AVL_TREEE.h"
#include "Team.h"
#include "player.h"

world_cup_t::world_cup_t()
{
	this->teams_tree = AVLtree<Team> ();
	this->players_by_id = AVLtree<Player> ();
	this->players_by_goals = AVLtree<Player> ();
	this->valid_teams_tree = AVLtree<Team_score>();
	this->top_scorer = nullptr;
}

world_cup_t::~world_cup_t()
{
	delete top_scorer;
}


StatusType world_cup_t::add_team(int teamId, int points)
{
	if(teamId < 0 || points < 0)
	{
		return StatusType::INVALID_INPUT;
	}

	Team* tmp_team = new Team(teamId,points);
	if(tmp_team == nullptr)
	{
		return StatusType::ALLOCATION_ERROR;
	}

	if(teams_tree.Find(*tmp_team) != nullptr)
	{
		return StatusType::FAILURE;
	}

	teams_tree.Insert(tmp_team);

	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId)
{
	if(teamId<=0)
	{
		return StatusType::INVALID_INPUT;
	}

	Team tmp_team = Team(teamId,0);
	if(&tmp_team == nullptr)
	{
		return StatusType::ALLOCATION_ERROR;
	}


	Team* team_to_delete = this->teams_tree.Find(tmp_team);
	if(team_to_delete == nullptr)
	{
		return StatusType::FAILURE;
	}

	if(team_to_delete == nullptr || team_to_delete->getNumOfPlayers() > 0)
	{
		return StatusType::FAILURE;
	}

	this->teams_tree.DeleteActiveNode(team_to_delete);

	this->teams_tree.DeleteNode(team_to_delete);
	
	return StatusType::FAILURE;
}

StatusType world_cup_t::add_player(int playerId, int teamId, int gamesPlayed,
                                   int goals, int cards, bool goalKeeper)
{
	if(playerId<=0 || teamId<= 0)
	{
		return StatusType::INVALID_INPUT;
	}

	Team* tmp_team = new Team(teamId,0);
	
	if(&tmp_team == nullptr)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	
	if(this->teams_tree.Find(*tmp_team) != nullptr)
	{
		return StatusType::FAILURE;
	}
	
	Team* target_team = this->teams_tree.Find(*tmp_team);
	
	Player* tmp_player = new Player(playerId,nullptr);
	if(this->players_by_id.Find(*tmp_player) != nullptr)
	{
		return StatusType::FAILURE;
	}
	
	Player* target_id_player = new Player(playerId, target_team, gamesPlayed, goals,  gamesPlayed, target_team->getGamesCounter(), cards, goalKeeper, nullptr, nullptr,SortByInfo::PLAYER_ID);

	if(&target_id_player == nullptr)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	
	Player* target_goal_player = new Player(playerId, target_team, gamesPlayed, goals, gamesPlayed, target_team->getGamesCounter() ,cards, goalKeeper, nullptr, nullptr,SortByInfo::GOALS);

	if(&target_goal_player == nullptr)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	
	this->players_by_id.Insert(target_id_player);
	this->players_by_goals.Insert(target_goal_player);

	target_team->setTotalGoals(goals);
	target_team->setTotalCards(cards);
	target_team->setPlayerNum(target_team->getNumOfPlayers() + 1);
	if(goals > target_team->getTopScorer()->getGoals())
	{
		target_team->setTopScorer(target_id_player);
	}

	if(target_id_player->getGoalkeeper() == true)
	{
		target_team->addGoalKeepersCtr();
		if(target_team->canParticipate() == false)
		{
			target_team->setGoalKeeper(true);
		}
	}

	if(target_team->getNumOfPlayers() >= 11 && target_team->canParticipate())
	{
		int score = target_team->getScore();
		Team_score* team_to_insert = new Team_score(teamId, score);
		this->valid_teams_tree.Insert(team_to_insert);
	}

	//insertion into team trees
	//------------------may need to change target_team to nullptr----------------------
	AVLnode<Player>* player_id_location = this->players_by_id.Find(this->players_by_id.getRoot(), *target_id_player);
	AVLnode<Player>* player_goals_location = this->players_by_goals.Find(this->players_by_goals.getRoot(), *target_goal_player);

	Player* player_id_to_insert_in_team = new Player(playerId, nullptr, gamesPlayed, goals, gamesPlayed, target_team->getGamesCounter() ,cards, goalKeeper, 
				player_goals_location, player_id_location, SortByInfo::PLAYER_ID);
	Player* player_goal_to_insert_in_team = new Player(playerId, nullptr, gamesPlayed, goals, gamesPlayed, target_team->getGamesCounter() ,cards, goalKeeper,
				player_goals_location, player_id_location, SortByInfo::GOALS);

	target_team->insertPlayer(player_id_to_insert_in_team, player_goal_to_insert_in_team);

	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_player(int playerId)
{
	if(playerId<=0)
	{
		return StatusType::INVALID_INPUT;
	}

	Player* tmp_player = new Player(playerId,nullptr);

	if(&tmp_player == nullptr)
	{
		return StatusType::ALLOCATION_ERROR;
	}

	if(this->players_by_id.Find(*tmp_player) != nullptr)
	{
		return StatusType::FAILURE;
	}

	Player* target_player_by_id = this->players_by_id.Find(*tmp_player);
	Player* target_player_by_goals = this->players_by_goals.Find(*tmp_player);

	Team* target_team = target_player_by_id->getPlayersTeam();
	bool flag_removed = false;

	if(target_player_by_id->getGoalkeeper())
	{
		target_team->subtractGoalKeepersCtr();
		if(target_team->getGoalkeepersCtr() == 0)
		{
			target_team->setGoalKeeper(false);

			//remove from valid tree and update stats
			int score = target_team->getScore();
			Team_score* team_to_remove = new Team_score(target_team->getTeamId(),score);
			this->valid_teams_tree.DeleteActiveNode(team_to_remove);
			flag_removed = true;
		}
	}

	if(target_team->getNumOfPlayers() == 11 && flag_removed == false)
	{
		//remove from valid tree and update stats
		int score = target_team->getScore();
		Team_score* team_to_remove = new Team_score(target_team->getTeamId(),score);
		this->valid_teams_tree.DeleteActiveNode(team_to_remove);
	}

	if(target_team->canParticipate() && target_team->getNumOfPlayers() >= 11)
	{
		//update stats
		int old_score = target_team->getScore();
		target_team->setTotalGoals(- target_player_by_id->getGoals());
		target_team->setTotalCards(- target_player_by_id->getCards());
		int new_score = old_score - target_player_by_id->getGoals() + target_player_by_id->getCards();
		Team_score* team_to_update = new Team_score(target_team->getTeamId(),old_score);

		Team_score* team_to_update_node = this->valid_teams_tree.Find(*team_to_update);
		team_to_update_node->score = new_score;
	}

	target_player_by_id->setTeam(nullptr);
	target_player_by_goals->setTeam(nullptr);

	this->players_by_goals.DeleteActiveNode(target_player_by_goals);
	this->players_by_id.DeleteActiveNode(target_player_by_id);

	target_team->removePlayer(target_player_by_id);

	return StatusType::SUCCESS;
}


StatusType world_cup_t::update_player_stats(int playerId, int gamesPlayed,
                                        int scoredGoals, int cardsReceived)
{
	if(playerId<=0 || gamesPlayed<0 || scoredGoals<0 || cardsReceived<0)
	{
		return StatusType::INVALID_INPUT;
	}
	
	Player player_to_find_id = Player(playerId, nullptr,0,0,false, SortByInfo::PLAYER_ID);
	if(&player_to_find_id == nullptr)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	
	Player* player_to_update_id = this->players_by_id.Find(player_to_find_id);
	if(player_to_update_id == nullptr)
	{
		return StatusType::FAILURE;
	}

	Player player_to_find_goals = Player(playerId, nullptr,0,0,false, SortByInfo::GOALS);
	if(&player_to_find_goals == nullptr)
	{
		return StatusType::ALLOCATION_ERROR;
	}

	Player* player_to_update_goals = this->players_by_goals.Find(player_to_find_goals);
	if(player_to_update_goals == nullptr)
	{
		return StatusType::FAILURE;
	}

	Team* current_team = player_to_update_id->getPlayersTeam();
	Player* inner_player_to_update_id = current_team->getPlayersById()->Find(player_to_find_id);
	Player* inner_player_to_update_goals = current_team->getPlayersByGoals()->Find(player_to_find_goals);

	//now we have the player in the 2 bigger trees and the 2 inner trees (in the team) and the players team
	//  1)player_to_update_id    2)player_to_update_goals    3)inner_player_to_update_id    4)inner_player_to_update_goals
	
	//updating player in player_id tree
	player_to_update_id->setGoals(scoredGoals);
	player_to_update_id->setcards(cardsReceived);
	player_to_update_id->setGamesPlayed(gamesPlayed);

	//updating team details
	current_team->setTotalGoals(scoredGoals);
	current_team->setTotalCards(cardsReceived);

	//updating player in inner player_id tree
	inner_player_to_update_id->setGoals(scoredGoals);
	inner_player_to_update_id->setcards(cardsReceived);
	inner_player_to_update_id->setGamesPlayed(gamesPlayed);

	//saving player details
	int player_goals = player_to_update_id->getGoals();
	int player_cards = player_to_update_id->getCards();
	bool is_goal_keeper = player_to_update_id->getGoalkeeper();

	//updating player in player_goals tree
	this->players_by_goals.DeleteActiveNode(player_to_update_goals);
	Player* updated_player_by_goals = new Player(playerId, current_team, player_to_update_id->getGamesPlayed(), player_goals, player_to_update_id->getGamesPlayedBeforeJoin(), 
													player_to_update_id->getInitialGames(), player_cards, player_to_update_id->getGoalkeeper(),
													nullptr, nullptr,  SortByInfo::GOALS);

	this->players_by_goals.Insert(updated_player_by_goals);
	AVLnode<Player>* new_player_goals_location = this->players_by_goals.Find(this->players_by_goals.getRoot(), *updated_player_by_goals);
	AVLnode<Player>* new_player_id_location = this->players_by_id.Find(this->players_by_id.getRoot(), *player_to_update_id);

	//updating player in inner player_goals tree
	this->players_by_goals.DeleteActiveNode(inner_player_to_update_goals);
	Player* updated_inner_player_by_goals = new Player(playerId, nullptr, player_to_update_id->getGamesPlayed(), player_goals, player_to_update_id->getGamesPlayedBeforeJoin(), 
													player_to_update_id->getInitialGames(), player_cards, player_to_update_id->getGoalkeeper(),
													new_player_goals_location, new_player_id_location, SortByInfo::GOALS);

	current_team->getPlayersByGoals()->Insert(updated_inner_player_by_goals);

	//updating pointers in inner player_id tree
	inner_player_to_update_id->setPlayerById(new_player_id_location);
	inner_player_to_update_id->setPlayerByGoal(new_player_goals_location);

	//updating top scorer for all players
	if(player_goals > this->top_scorer->getGoals())
	{
		this->top_scorer = player_to_update_id;
	}
	else{
		if(player_goals == this->top_scorer->getGoals())
		{
			if(player_cards < this->top_scorer->getCards())
			{
				this->top_scorer = player_to_update_id;
			}
			else
			{
				if(player_cards == this->top_scorer->getCards())
				{
					if(playerId > this->top_scorer->getID())
					{
						this->top_scorer = player_to_update_id;
					}
				}
			}
		}
	}


	//updating top scorer for the team
	if(player_goals > current_team->getTopScorer()->getGoals())
	{
		current_team->setTopScorer(player_to_update_id);
	}
	else{
		if(player_goals == current_team->getTopScorer()->getGoals())
		{
			if(player_cards < current_team->getTopScorer()->getCards())
			{
				current_team->setTopScorer(player_to_update_id);
			}
			else
			{
				if(player_cards == current_team->getTopScorer()->getCards())
				{
					if(playerId > this->top_scorer->getID())
					{
						current_team->setTopScorer(player_to_update_id);
					}
				}
			}
		}
	}

	/*
	//updating team in valid teams tree
	Team_score* tmp_team = new Team_score(current_team->getTeamId(), current_team->getScore());
	AVLnode<Team_score>* team_to_update = this->valid_teams_tree.Find(this->valid_teams_tree.getRoot(), *tmp_team);
	team_to_update->info->score = team_to_update->info->score + scoredGoals - cardsReceived;
	// int new_goals = player_to_update_id->getGoals();
	// int new_cards = player_to_update_id->getCards();
	// int new_games_played = player_to_update_id->getCards();

	// player_to_update_goals->setGoals(scoredGoals);
	// player_to_update_goals->setcards(cardsReceived);
	// player_to_update_goals->setGamesPlayed(gamesPlayed);

	int player_goals = player_to_update_id->getGoals();
	int player_cards = player_to_update_id->getCards();
	bool is_goal_keeper = player_to_update_id->getGoalkeeper();
	
	AVLnode<Player>* tmp_player_ptr = current_team->getPlayersByGoals()->Find(current_team->getPlayersByGoals()->getRoot(), *player_to_update_goals);
	//int player_id = player_to_update_id->getID();

	this->players_by_goals.DeleteActiveNode(player_to_update_goals);
	Player* updated_player_by_goals = new Player(playerId, current_team, player_to_update_id->getGamesPlayed(), player_goals,
													player_to_update_id->getGamesPlayedBeforeJoin(), player_to_update_id->getInitialGames(), player_cards,
													 is_goal_keeper, )

	
	if(player_goals > this->top_scorer->getGoals())
	{
		this->top_scorer = player_to_update_id;
	}
	else{
		if(player_goals == this->top_scorer->getGoals())
		{
			if(player_cards < this->top_scorer->getCards())
			{
				this->top_scorer = player_to_update_id;
			}
			else
			{
				if(player_cards == this->top_scorer->getCards())
				{
					if(player_id > this->top_scorer->getID())
					{
						this->top_scorer = player_to_update_id;
					}
				}
			}
		}
	}

	

	
	player_to_update_id = current_team->getPlayersById()->Find(player_to_find_id);
	player_to_update_goals = current_team->getPlayersByGoals()->Find(player_to_find_goals);

	//update team in valid teams tree
	int old_score = current_team->getScore();
	int new_score = old_score + scoredGoals - cardsReceived;
	Team_score* team_to_find = new Team_score(current_team->getTeamId(), old_score);
	Team_score* team_score_to_update = this->valid_teams_tree.Find(*team_to_find);
	team_score_to_update->score = new_score;

	//updating player in the team inner trees
	current_team->setTotalCards(cardsReceived);
	current_team->setTotalGoals(scoredGoals);

	if(player_goals > current_team->getTopScorer()->getGoals())
	{
		current_team->setTopScorer(player_to_update_id);
	}
	else{
		if(player_goals == current_team->getTopScorer()->getGoals())
		{
			if(player_cards < current_team->getTopScorer()->getCards())
			{
				current_team->setTopScorer(player_to_update_id);
			}
			else
			{
				if(player_cards == current_team->getTopScorer()->getCards())
				{
					if(player_id > this->top_scorer->getID())
					{
						current_team->setTopScorer(player_to_update_id);
					}
				}
			}
		}
	}

	

	player_to_update_id->setGoals(scoredGoals);
	player_to_update_id->setcards(cardsReceived);
	player_to_update_id->setGamesPlayed(gamesPlayed);

	player_to_update_goals->setGoals(scoredGoals);
	player_to_update_goals->setcards(cardsReceived);
	player_to_update_goals->setGamesPlayed(gamesPlayed);
	*/
	return StatusType::SUCCESS;
}

StatusType world_cup_t::play_match(int teamId1, int teamId2)
{
	if(teamId1<=0 || teamId2<=0 || teamId1==teamId2)
	{
		return StatusType::INVALID_INPUT;
	}

	
	Team team1_tmp  = Team(teamId1,0);
	if(&team1_tmp == nullptr)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	
	Team team2_tmp  = Team(teamId2,0);
	if(&team2_tmp == nullptr)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	Team* team1 = this->teams_tree.Find(team1_tmp);
	Team* team2 = this->teams_tree.Find(team2_tmp);

	if(team1 == nullptr || team2 == nullptr)
	{
		return StatusType::FAILURE;
	}

	if(team1->getNumOfPlayers() < 11 || team2->getNumOfPlayers() < 11
			|| ! team1->canParticipate() || ! team2->canParticipate())
			{
				return StatusType::FAILURE;
			}

	team1->bumpGamesCounter();
	team2->bumpGamesCounter();

	int team1_score = team1->getScore();
	int team2_score = team2->getScore();
	if(team1_score == team2_score)
	{
		Team_score* team1_to_find = new Team_score(teamId1,team1_score);
		Team_score* team1_to_update = this->valid_teams_tree.Find(*team1_to_find);

		Team_score* team2_to_find = new Team_score(teamId2,team2_score);
		Team_score* team2_to_update = this->valid_teams_tree.Find(*team2_to_find);

		team1_to_update->score += 1;
		team2_to_update->score += 1;
		//Team_score* team2_to_update = this->valid_teams_tree.Find(*(new Team_score(teamId2,team2_score)));

		team1->setPoints(1);
		team2->setPoints(1);

	}
	else
	{
		if(team1_score < team2_score)
		{
			team2->setPoints(3);
			Team_score* team2_to_find = new Team_score(teamId2,team2_score);
			Team_score* team2_to_update = this->valid_teams_tree.Find(*team2_to_find);
			team2_to_update->score += 3;
		}
		else
		{
			team1->setPoints(3);
			Team_score* team1_to_find = new Team_score(teamId1,team1_score);
			Team_score* team1_to_update = this->valid_teams_tree.Find(*team1_to_find);
			team1_to_update->score += 3;
		}
	}


	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_num_played_games(int playerId)
{
	if(playerId <= 0)
	{
		return StatusType::INVALID_INPUT;
	}

	Player tmp_player = Player(playerId,nullptr);
	if(&tmp_player == nullptr)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	Player* player_to_find = this->players_by_id.Find(tmp_player);

	if(player_to_find == nullptr)
	{
		return StatusType::FAILURE;
	}

	Team* current_team = player_to_find->getPlayersTeam();
	int team_games = current_team->getGamesCounter();

	return player_to_find->getGamesPlayed() + team_games - player_to_find->getInitialGames();

	//return 22;
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
	if(teamId <= 0)
	{
		return StatusType::INVALID_INPUT;
	}

	Team tmp_team = Team(teamId,0);
	if(&tmp_team == nullptr)
	{
		return StatusType::ALLOCATION_ERROR;
	}

	Team* target_team = this->teams_tree.Find(tmp_team);

	if(target_team == nullptr)
	{
		return StatusType::FAILURE;
	}

	return target_team->getTotalPoints();

	//return 30003;
}

StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId)
{
	if(teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2 || newTeamId <=0)  //////////here/////////////////
	{
		return StatusType::INVALID_INPUT;
	} 

	Team team1_tmp  = Team(teamId1);
	Team team2_tmp  = Team(teamId2);

	Team* team1 = this->teams_tree.Find(team1_tmp);
	Team* team2 = this->teams_tree.Find(team2_tmp);
	if(team1 == nullptr || team2 == nullptr)
	{
		return StatusType::FAILURE;
	}

	Team tmp_new_team = Team(newTeamId);
	Team* tmp_team_with_new_id = this->teams_tree.Find(tmp_new_team);
	if(newTeamId != teamId1 && newTeamId !=teamId2 && tmp_team_with_new_id != nullptr)
	{
		return  StatusType::FAILURE;
	}

	/* Now we get all the players from the two teams into arrays and merge them into
			a new array that includes both of the teams players
		
		first the proccess is made for the Id tree of the players
	*/

	/// first we made the modifications on the tree sorted by the id of the players
	
	//helper func
	Player* arr_player_id_team1 = (Player*)malloc(sizeof(Player)* team1->getNumOfPlayers()); 
	putTreeInsideArr(team1->getPlayersById()->getRoot(),0, arr_player_id_team1);
	
	Player* arr_player_id_team2 = (Player*)malloc(sizeof(Player)* team2->getNumOfPlayers()); 
	putTreeInsideArr(team2->getPlayersById()->getRoot(),0, arr_player_id_team2);


	Player* arr_player_id_comb = (Player*)malloc(sizeof(Player)* (team2->getNumOfPlayers() + team1->getNumOfPlayers())); 
	
	Team new_team= Team(newTeamId);
	new_team.setPlayerNum(team2->getNumOfPlayers() + team1->getNumOfPlayers());
	//new_team.setTotalCards(team1->getTotalCards()+ team2->getTotalCards());
	//new_team.setTotalGoals(team1->getTotalGoals()+ team2->getTotalGoals());
	if(team1->getTopScorer() > team2->getTopScorer())
	{
		new_team.setTopScorer(team1->getTopScorer());
	}
	else
	{
		new_team.setTopScorer(team2->getTopScorer());
	}
	
	new_team.setPoints(team1->getTotalPoints()+team1->getTotalPoints());
	new_team.setGoalkeeperCtr(team1->getGoalkeepersCtr()+ team2->getGoalkeepersCtr());

	
	// put both arrays in the same array by sorted order, and with updating the team ptr of each player
	combineArrays(arr_player_id_team1, arr_player_id_team2, arr_player_id_comb,team1->getNumOfPlayers(),team2->getNumOfPlayers());
	AVLtree<Player>* newTree_players_by_id = nullptr;
	
	//building the actual new tree
	AVLnode<Player>* new_root_by_id = putArrayInsideTree(arr_player_id_comb,0,team2->getNumOfPlayers() + team1->getNumOfPlayers());
	AVLtree<Player>* new_tree_by_id = makeTreeOutOfNode(new_root_by_id, (team2->getNumOfPlayers() + team1->getNumOfPlayers()));

	//updating to the new players id tree inside the team
	new_team.setPlayersById(new_tree_by_id);

	// the same process is made for the players by goals
	Player* arr_player_goals_team1 = (Player*)malloc(sizeof(Player)* team1->getNumOfPlayers()); 
	putTreeInsideArr(team1->getPlayersByGoals()->getRoot(),0, arr_player_goals_team1);
	
	Player* arr_player_goals_team2 = (Player*)malloc(sizeof(Player)* team2->getNumOfPlayers()); 
	putTreeInsideArr(team2->getPlayersByGoals()->getRoot(),0, arr_player_goals_team2);


	Player* arr_player_goals_comb = (Player*)malloc(sizeof(Player)* (team2->getNumOfPlayers() + team1->getNumOfPlayers())); 
	
	// put both arrays in the same array by sorted order, and with updating the team ptr of each player
	combineArrays(arr_player_goals_team1, arr_player_goals_team2, arr_player_goals_comb,team1->getNumOfPlayers(),team2->getNumOfPlayers());
	AVLtree<Player>* newTree_players_by_goals = nullptr;
	AVLnode<Player>* new_root_by_goals = putArrayInsideTree(arr_player_goals_comb,0,team2->getNumOfPlayers() + team1->getNumOfPlayers());
	AVLtree<Player>* new_tree_by_goals = makeTreeOutOfNode(new_root_by_goals, (team2->getNumOfPlayers() + team1->getNumOfPlayers()));
	
	new_team.setPlayersByGoals(new_tree_by_goals);

	/// the team is now ready ! wa7d z8rota
	
	//
	// now we delete the two teams from the teams tree and add the new one in
	//

	remove_team(teamId1);
	remove_team(teamId2);

	this->teams_tree.Insert(&new_team);	


	return StatusType::SUCCESS;
}



AVLtree<Player>* makeTreeOutOfNode(AVLnode<Player>* new_root, int numOfElemnts)
{
	AVLtree<Player> new_tree = AVLtree<Player>();
	new_tree.setRoot(new_root);
	new_tree.setNumOfElements(numOfElemnts);
	new_tree.setMinElement(new_tree.FindMaxElement(new_root));
	new_tree.setMaxElement(new_tree.FindMinElement(new_root));
	
	return &new_tree;
	
}


void world_cup_t::putTreeInsideArr(AVLnode<Player>* current_node, int index,Player arr[], Team* current_team)
{
	 if (current_node == nullptr)
    {
        return;
    }

    putTreeInsideArr(current_node->left, index, arr, current_team);
	current_node->Info().getPlayerByIdLocaton()->Info().setTeam(current_team);
	current_node->Info().getPlayerByGoalLocation()->Info().setTeam(current_team);
    arr[index] = *current_node->InfoPtr();
    putTreeInsideArr(current_node->right, index + 1, arr, current_team);
	return ;
}

void combineArrays(Player arr1[], Player arr2[], Player new_arr[], int n1, int n2)
{
	int i1 = 0, i2 = 0, i3 = 0;

	while(i1 < n1 && i2 < n2)
	{
		if(arr1[i1] < arr2[i2])
		{
			new_arr[i3] = arr1[i1];
			i1++;
			i3++;
		}
		else
		{
			new_arr[i3] = arr1[i2];
			i2++;
			i3++;
		}
	}

	while(i1 < n1)
	{
		new_arr[i3] = arr1[i1];
		i1++;
		i3++;
	}

	while(i2 < n2)
	{
		new_arr[i3] = arr1[i2];
		i2++;
		i3++;
	}

	return;
}

AVLnode<Player>* putArrayInsideTree(Player array[], int low, int high)
{
	if(low > high)
	{
		return nullptr;
	}
	
	int middle = (low + high)/2;
	AVLnode<Player>* current_node = nullptr;
	current_node->setInfo(&array[middle]);

	current_node->setLeft(putArrayInsideTree(array, low, middle - 1));
	if(current_node->Left())
	{
		current_node->Left()->setParent(current_node);
	}

	current_node->setRight(putArrayInsideTree(array, middle + 1, high));
	if(current_node->Right())
	{
		current_node->Right()->setParent(current_node);
	}
	return;
}

output_t<int> world_cup_t::get_top_scorer(int teamId)
{
	if(teamId == 0)
	{
		return StatusType::INVALID_INPUT;
	}

	if(teamId<0)
	{
		return this->top_scorer->getGoals();
	}

	Team tmp_team = Team(teamId,0);
	if(&tmp_team == nullptr)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	Team* target_team = this->teams_tree.Find(tmp_team);

	if(target_team == nullptr)
	{
		return StatusType::FAILURE;
	}

	return target_team->getTopScorer()->getGoals();
	
	//return 2008;
}

output_t<int> world_cup_t::get_all_players_count(int teamId)
{
	if(teamId == 0)
	{
		return StatusType::INVALID_INPUT;
	}

	if(teamId<0)
	{
		return this->players_by_id.NumOfElements();
	}

	Team tmp_team = Team(teamId,0);
	if(&tmp_team == nullptr)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	Team* target_team = this->teams_tree.Find(tmp_team);

	if(target_team == nullptr)
	{
		return StatusType::FAILURE;
	}

	return target_team->getNumOfPlayers();
    //static int i = 0;
    //return (i++==0) ? 11 : 2;
}


StatusType world_cup_t::get_all_players(int teamId, int *const output)
{

	if(teamId == 0 || output == nullptr)
	{
		return StatusType::INVALID_INPUT;
	}
	

	if(teamId > 0)
	{
		
		Team tmp_team = Team(teamId,0);
		if(&tmp_team == nullptr)
		{
			return StatusType::ALLOCATION_ERROR;
		}

		Team* target_team = this->teams_tree.Find(tmp_team);

		if(target_team == nullptr)
		{
			return StatusType::FAILURE;
		}

		if(target_team->getNumOfPlayers() == 0)
		{
			return StatusType::FAILURE;
		}
		
		int* players_arr = (int *)malloc(sizeof(int)*(target_team->getNumOfPlayers()));
		if(players_arr == nullptr)
		{
			return StatusType::ALLOCATION_ERROR;
		}

		//AVLtree<Player>* players_by_goals_dup = new AVLtree<Player>(*(target_team->getPlayersByGoals()));
		AVLnode<Player>* player_root = (players_by_goals.getRoot());
		
		putTreeInArr(player_root, players_arr,0);
		output[0] = *players_arr;

		return StatusType::SUCCESS;
	}

	if(teamId<0)
	{
		if(this->players_by_goals.getRoot() == nullptr)
		{
			return StatusType::FAILURE;
		}
		
		int* players_arr = (int*)malloc(sizeof(int)*(this->players_by_goals.NumOfElements()));
		if(players_arr == nullptr)
		{
			return StatusType::ALLOCATION_ERROR;
		}

		//AVLtree<Player>* players_by_goals_dup = new AVLtree<Player>((this->players_by_goals));
		AVLnode<Player>* player_root = (players_by_goals.getRoot());
		putTreeInArr(player_root, players_arr,0);

		output[0] = *players_arr;

		return StatusType::SUCCESS;
	}


	// TODO: Your code goes here
    // //output[0] = 4001;
    // //output[1] = 4002;
	return StatusType::FAILURE;
}


// note: the players in the goals tree is set in order in the tree by the 
// operator given in AVL tree, and the operator is set to tmatch the requirements
// of the function get all players
void putTreeInArr(AVLnode<Player>* root, int* players_arr, int counter)
{	
	putTreeInArr(root->left, players_arr,counter);
	players_arr[counter] = root->InfoPtr()->getID();
	
	putTreeInArr(root->right, players_arr, counter++);
	players_arr[counter] = root->InfoPtr()->getID();

}



//next
output_t<int> world_cup_t::get_closest_player(int playerId, int teamId)
{
	/*legal check*/
	if(playerId <=0 || teamId <=0)
	{
		return StatusType::INVALID_INPUT; 
	}

	Team tmp_team = Team(teamId,0);
	if(&tmp_team == nullptr)
	{
		return StatusType::ALLOCATION_ERROR;
	}

	Team* target_team = this->teams_tree.Find(tmp_team);
	if(target_team == nullptr)
	{
		return StatusType::FAILURE;
	}

	Player* tmp_player = new Player(playerId,target_team);
	if(&tmp_player == nullptr)
	{
		return StatusType::ALLOCATION_ERROR;
	}

	if(target_team->getPlayersById()->Find(*tmp_player) == nullptr)
	{
		return StatusType::FAILURE;
	}
	
		// one or less players in total.
	if(this->players_by_id.NumOfElements() <= 1)
	{
		return StatusType::FAILURE;
	}
	/*end of legal check*/
	//---------------------------------------------------------//

	AVLnode<Player>* target_player = (target_team->getPlayersByGoals()->Find(target_team->getPlayersByGoals()->getRoot(),*tmp_player));

	AVLnode<Player>* left_son = target_player->left;
	AVLnode<Player>* right_son = target_player->right;
	AVLnode<Player>* parent_of_player = target_player->parent;

	if(parent_of_player == nullptr)
	{
		return left_son->InfoPtr()->getID();
	}
	
	if(left_son == nullptr && right_son == nullptr)
	{
		return parent_of_player->InfoPtr()->getID();;
	}

	int delta_goals_left = target_player->InfoPtr()->getGoals() - left_son->InfoPtr()->getGoals();
	int delta_goals_right = target_player->InfoPtr()->getGoals() - right_son->InfoPtr()->getGoals();
	int delta_goals_parent = target_player->InfoPtr()->getGoals() - parent_of_player->InfoPtr()->getGoals(); 
	
	AVLnode<Player>* equal_node_1 = nullptr;
	AVLnode<Player>* equal_node_2 = nullptr;

	/// dont forget mikra ktsee


	/*
		start of stage #1
	*/
	if(delta_goals_left < delta_goals_right && delta_goals_left < delta_goals_parent)
	{
		return left_son->InfoPtr()->getID();
	}
	if(delta_goals_right < delta_goals_left && delta_goals_right < delta_goals_parent)
	{
		return right_son->InfoPtr()->getID();
	}
	if(delta_goals_parent < delta_goals_right && delta_goals_parent < delta_goals_left)
	{
		return parent_of_player->InfoPtr()->getID();
	}
	
	/*
		start of stage #2
		this handles the case when two nodes have the same delta goals
	*/

	if(delta_goals_parent == delta_goals_right && delta_goals_parent < delta_goals_left)
	{
		AVLnode<Player>* equal_node_1 = target_player->parent;
		AVLnode<Player>* equal_node_2 = target_player->right;
	}

	if(delta_goals_left == delta_goals_right && delta_goals_left < delta_goals_parent)
	{
		AVLnode<Player>* equal_node_1 = target_player->left;
		AVLnode<Player>* equal_node_2 = target_player->right;
	}

	int delta_cards_1 = target_player->InfoPtr()->getCards() - equal_node_1->InfoPtr()->getCards();
	int delta_cards_2 = target_player->InfoPtr()->getCards() - equal_node_2->InfoPtr()->getCards();

	if(delta_cards_1 < delta_cards_2)
	{
		return equal_node_1->InfoPtr()->getID();
	}
	
	if(delta_cards_1 > delta_cards_2)
	{
		return equal_node_2->InfoPtr()->getID();
	}

	/*
		start of stage #3
		this handles the case when two nodes have the same delta cards
	*/

	if(delta_cards_1 == delta_cards_2)
	{

		int delta_Id_1 = target_player->InfoPtr()->getID() - equal_node_1->InfoPtr()->getID();
		int delta_Id_2 = target_player->InfoPtr()->getID() - equal_node_2->InfoPtr()->getID();
		if(delta_Id_1 < delta_Id_2)
		{
			return equal_node_1->InfoPtr()->getID();
		}
		
		if(delta_Id_1 > delta_Id_2)
		{
			return equal_node_2->InfoPtr()->getID();
		}

		if(delta_Id_1 == delta_Id_2)
		{
			if(equal_node_1->InfoPtr()->getID() > equal_node_2->InfoPtr()->getID())
			{
				return equal_node_1->InfoPtr()->getID();
			}
			else
			{
				return equal_node_2->InfoPtr()->getID();
			}
		
		}

	}

	return 1006;
}

output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId)
{
	if(maxTeamId<0 || minTeamId<0 || maxTeamId<minTeamId)
	{
		return StatusType::INVALID_INPUT;
	}

	int r_ctr = count_wanted_teams(minTeamId,maxTeamId,this->valid_teams_tree.getRoot());

	Team_score* competition = (Team_score*)malloc(sizeof(Team_score)*r_ctr);
	if(competition == nullptr)
	{
		return StatusType::ALLOCATION_ERROR;
	}

	get_wanted_teams(minTeamId, maxTeamId, this->valid_teams_tree.getRoot(), 0, competition);

	int rounds_ctr = 1;
	int index = 0;
	

	while(r_ctr/rounds_ctr >= 1)
	{
		index = 0;
		for(int i=0 ; i < roundUp(r_ctr,rounds_ctr) ; i+2)
		{
			if(i+1 >= roundUp(r_ctr,rounds_ctr))
			{
				competition[index] = competition[i];
			}
			else
			{
				if(competition[i].score > competition[i+1].score)
				{
					competition[index] = competition[i];
					competition[index].score+=3;
					index++;
				}
				else
				{
					if(competition[i].score < competition[i+1].score)
					{
						competition[index] = competition[i+1];
						competition[index].score+=3;
						index++;
					}
					else
					{
						if(competition[i].teamId > competition[i+1].teamId)
						{
							competition[index] = competition[i];
							competition[index].score+=3;
							index++;
						}
						else
						{
							competition[index] = competition[i+1];
							competition[index].score+=3;
							index++;
						}
					}
				}
				
			}
		}
		
		rounds_ctr++;
		
		if(roundUp(r_ctr,rounds_ctr) == 1)
		{
			int winner = competition[0].teamId;
			free(competition);
			return winner;
		}
	}
	

	//return 2;
}


int world_cup_t::roundUp(int a, int b)
{
	if(a/b == a-(a/b))
	{
		return a/b;
	}
	return a/b + 1;
}


int world_cup_t::count_wanted_teams(int minTeamID, int maxTeamID, AVLnode<Team_score>* current_team)
{
	if(current_team == nullptr ||current_team->info->teamId < minTeamID || current_team->info->teamId < minTeamID)
	{
		return 0;
	}

	return 1 + count_wanted_teams(minTeamID,maxTeamID,current_team->left) + count_wanted_teams(minTeamID,maxTeamID,current_team->right);
}


void world_cup_t::get_wanted_teams(int minTeamID, int maxTeamID, AVLnode<Team_score>* current_team, int index, Team_score team_array[])
{
	if(current_team == nullptr ||current_team->info->teamId < minTeamID || current_team->info->teamId < minTeamID)
	{
		return;
	}

	get_wanted_teams(minTeamID, maxTeamID, current_team->left, index ,team_array);

	team_array[index] = Team_score(current_team->info->teamId, current_team->info->score);

	get_wanted_teams(minTeamID, maxTeamID, current_team->right, index + 1, team_array);
}
/*void World_cup_t::clearTeams(AVLnode<Team> team_tree, Player player)
{
	if(*team_tree == nullptr)
	{
		return;
	}
	else
	{
		
		
	}

}*/
