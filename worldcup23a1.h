// 
// 234218 Data Structures 1.
// Semester: 2023A (winter).
// Wet Exercise #1.
// 
// Recommended TAB size to view this file: 8.
// 
// The following header file contains all methods we expect you to implement.
// You MAY add private methods and fields of your own.
// DO NOT erase or modify the signatures of the public methods.
// DO NOT modify the preprocessors in this file.
// DO NOT use the preprocessors in your other code files.
// 

#ifndef WORLDCUP23A1_H_
#define WORLDCUP23A1_H_

#include "wet1util.h"
#include "AVL_TREEE.h"
#include "Team.h"
#include "player.h"

struct Team_score
{
	int teamId;
	int score;

	Team_score(int teamID, int score):teamId(teamId), score(score){} 
	~Team_score() = default;

	bool operator<(Team_score other_team)
	{
		return this->score < other_team.score;
	}

	bool operator>(Team_score other_team)
	{
		return this->score > other_team.score;
	}

	bool operator==(Team_score other_team)
	{
		return this->teamId == other_team.teamId && this->score == other_team.score;
	}

	Team_score& operator=(const Team_score& other_team) = default;
	
};



class world_cup_t {
private:
	AVLtree<Team> teams_tree;
	AVLtree<Player> players_by_id;
	AVLtree<Player> players_by_goals;
	AVLtree<Team_score> valid_teams_tree;
	Player* top_scorer;
	
public:
	//helper functions 

	void putTreeInArr(AVLnode<Player>* root, int* players_arr,int counter);

	// <DO-NOT-MODIFY> {
	
	world_cup_t();
	virtual ~world_cup_t();
	
	//update for teams_score tree
	StatusType add_team(int teamId, int points);
	
	StatusType remove_team(int teamId);
	
	//update for teams_score tree
	StatusType add_player(int playerId, int teamId, int gamesPlayed,
	                      int goals, int cards, bool goalKeeper);
	
	//update for teams_score tree
	StatusType remove_player(int playerId);
	
	StatusType update_player_stats(int playerId, int gamesPlayed,
	                                int scoredGoals, int cardsReceived);
	
	StatusType play_match(int teamId1, int teamId2);
	
	output_t<int> get_num_played_games(int playerId);
	
	output_t<int> get_team_points(int teamId);
	
	//update for teams_score tree
	StatusType unite_teams(int teamId1, int teamId2, int newTeamId);
	
	output_t<int> get_top_scorer(int teamId);
	
	output_t<int> get_all_players_count(int teamId);
	
	StatusType get_all_players(int teamId, int *const output);
	
	output_t<int> get_closest_player(int playerId, int teamId);
	
	output_t<int> knockout_winner(int minTeamId, int maxTeamId);
	
	// } </DO-NOT-MODIFY>

	//returns round up value of a/b
	int roundUp(int a, int b);

	//inorder traversal to count the teams in the Knockout function
	int count_wanted_teams(int minTeamID, int maxTeamID, AVLnode<Team_score>* current_team);

	//inorder traversal to insert the target teams in an array
	//Note: inorder traversal = teams sorted from lowest to highest by ID in the array
	void get_wanted_teams(int minTeamID, int maxTeamID, AVLnode<Team_score>* current_team, int index, Team_score team_array[]);

	void putTreeInsideArr(AVLnode<Player>* current_node, int index, Player* arr[]);

	void combineArrays(Player* arr1[], Player* arr2[], Player* new_arr[], int n1, int n2);
};

#endif // WORLDCUP23A1_H_
