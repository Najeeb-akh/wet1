#include "Team.h"
#include ""

Team::Team(int TeamId, int points): TeamId(TeamId), total_points(points), players_num(0), 
            total_goals(0), total_cards(0), has_goalKeeper(false)
{
    this->players_by_id = nullptr;
    this->players_by_goals = nullptr;

    this->top_scorer = nullptr;
}

Team::Team(const Team& other_team)
{
    this->TeamId = other_team.TeamId;
    this->
}

