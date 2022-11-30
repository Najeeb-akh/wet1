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
	
	Player* target_id_player = new Player(playerId, target_team, gamesPlayed, goals, target_team->getGamesCounter(), gamesPlayed ,cards,
												goalKeeper,SortByInfo::PLAYER_ID);
	if(&target_id_player == nullptr)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	
	Player* target_goal_player = new Player(playerId, target_team, gamesPlayed, goals, target_team->getGamesCounter(), gamesPlayed ,cards,
												goalKeeper,SortByInfo::GOALS);

	if(&target_goal_player == nullptr)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	
	this->players_by_id.Insert(target_id_player);
	this->players_by_goals.Insert(target_goal_player);

	target_team->insertPlayer(target_id_player, target_goal_player);

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

	Player* target_player = this->players_by_id.Find(*tmp_player);
	this->players_by_goals.DeleteActiveNode(target_player);
	this->players_by_id.DeleteActiveNode(target_player);

	Team* target_team = target_player->getPlayersTeam();

	target_team->removePlayer(target_player);

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

	//updating player in player trees
	player_to_update_id->setGoals(scoredGoals);
	player_to_update_id->setcards(cardsReceived);
	player_to_update_id->setGamesPlayed(gamesPlayed);

	player_to_update_goals->setGoals(scoredGoals);
	player_to_update_goals->setcards(cardsReceived);
	player_to_update_goals->setGamesPlayed(gamesPlayed);

	int player_goals = player_to_update_id->getGoals();
	int player_cards = player_to_update_id->getCards();
	int player_id = player_to_update_id->getID();

	//-----------------check comparitson-----------------
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

	

	Team* current_team = player_to_update_id->getPlayersTeam();
	player_to_update_id = current_team->getPlayersById()->Find(player_to_find_id);
	player_to_update_goals = current_team->getPlayersByGoals()->Find(player_to_find_goals);

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
	player_to_update_id->setGamesPlayed(gamesPlayed);;

	player_to_update_goals->setGoals(scoredGoals);
	player_to_update_goals->setcards(cardsReceived);
	player_to_update_goals->setGamesPlayed(gamesPlayed);;
	
	return StatusType::SUCCESS;
}

StatusType world_cup_t::play_match(int teamId1, int teamId2)
{
	if(teamId1<=0 || teamId2<=0 || teamId1==teamId2)
	{
		return StatusType::INVALID_INPUT;
	}

	//----------------constructor doestn take one parameter------------
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

	team1->setGamesCounter();
	team2->setGamesCounter();

	int team1_score = team1->getTotalPoints() + team1->getTotalGoals() - team1->getTotalCards();
	int team2_score = team2->getTotalPoints() + team2->getTotalGoals() - team2->getTotalCards();
	if(team1_score == team2_score)
	{
		team1->setPoints(1);
		team2->setPoints(1);
	}
	else
	{
		if(team1_score < team2_score)
		{
			team2->setPoints(3);
		}
		else
		{
			team1->setPoints(3);
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
	// TODO: Your code goes here
	return StatusType::SUCCESS;
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
    //output[0] = 4001;
    //output[1] = 4002;
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
	// TODO: Your code goes here
	return 2;
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
