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
	Team* team_to_delete = this->teams_tree.Find(tmp_team);
	if(team_to_delete == nullptr)
	{
		return StatusType::ALLOCATION_ERROR;
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
	Team* tmp_team = new Team(teamId,0);
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
	Player* target_goal_player = new Player(playerId, target_team, gamesPlayed, goals, target_team->getGamesCounter(), gamesPlayed ,cards,
												goalKeeper,SortByInfo::GOALS);

	this->players_by_id.Insert(target_id_player);
	this->players_by_goals.Insert(target_goal_player);

	target_team->setTotalGoals(goals);
	target_team->setTotalCards(cards);
	target_team->setPlayerNum(target_team->getNumOfPlayers() + 1);
	if(goals > target_team->getTopScorer()->getGoals())
	{
		target_team->setTopScorer(target_id_player);
	}

	if(target_team->canParticipate() == false && target_id_player->getGoalkeeper() == true)
	{
		target_team->
	}



	target_team->insertPlayer(target_id_player, target_goal_player);

	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_player(int playerId)
{
	Player* tmp_player = new Player(playerId,nullptr);
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
	Player* player_to_update_id = this->players_by_id.Find(player_to_find_id);
	if(player_to_update_id == nullptr)
	{
		return StatusType::FAILURE;
	}

	Player player_to_find_goals = Player(playerId, nullptr,0,0,false, SortByInfo::GOALS);
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
	player_to_update_id->setGamesPlayed(gamesPlayed);

	player_to_update_goals->setGoals(scoredGoals);
	player_to_update_goals->setcards(cardsReceived);
	player_to_update_goals->setGamesPlayed(gamesPlayed);
	
	return StatusType::SUCCESS;
}

StatusType world_cup_t::play_match(int teamId1, int teamId2)
{
	if(teamId1<=0 || teamId2<=0 || teamId1==teamId2)
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

	if(team1->getNumOfPlayers() <= 11 || team2->getNumOfPlayers() <= 11
			|| ! team1->canParticipate() || ! team2->canParticipate())
			{
				return StatusType::FAILURE;
			}

	team1->bumpGamesCounter();
	team2->bumpGamesCounter();

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
	if(teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2 || newTeamId <=0)
	{
		return StatusType::INVALID_INPUT;
	} 

	Team team1_tmp  = Team(teamId1);
	Team team2_tmp  = Team(teamId2);

	Team* team1 = this->teams_tree.Find(team1_tmp);
	Team* team2 = this->teams_tree.Find(team2_tmp);

	
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
	// TODO: Your code goes here
    // output[0] = 4001;
    // output[1] = 4002;
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_closest_player(int playerId, int teamId)
{
	// TODO: Your code goes here
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


void world_cup_t::get_wanted_teams(int minTeamID, int maxTeamID, AVLnode<Team_score>* current_team, int index, Team_score* team_array)
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
