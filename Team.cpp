#include "Team.h"
#include "Player.h"

// Team::Team(int TeamId, int points): TeamId(TeamId), total_points(points), players_num(0), 
//             total_goals(0), total_cards(0),games_counter(0), has_goalKeeper(false)
// {
//     this->players_by_id = nullptr;
//     this->players_by_goals = nullptr;

//     this->top_scorer = nullptr;
// }

Team::Team(int TeamId, int points, int player_num, bool has_goalkeeper,
                int total_goals, int total_cards, int games_counter , int goalkeeper_ctr,
                  Player* top_scorer, AVLtree<Player>* players_by_id, AVLtree<Player>* players_by_goals)
                {
                    this->TeamId = TeamId;
                    this->total_points = points;
                    this->players_num = player_num;
                    this->has_goalKeeper = has_goalKeeper;
                    this->total_goals = total_goals;
                    this->total_cards = total_cards;
                    this->games_counter = games_counter;
                    this->top_scorer = top_scorer;
                    this->players_by_id = players_by_id;
                    this->goalKeepersCtr = goalkeeper_ctr;
                    this->players_by_goals = players_by_goals;
                }


Team::Team(const Team& other_team)
{
    this->TeamId = other_team.TeamId;
    this->total_points = other_team.total_points;
    this->players_num = other_team.players_num;
    this->has_goalKeeper = other_team.has_goalKeeper;
    this->total_goals = other_team.total_goals;
    this->total_cards = other_team.total_cards;
    this->top_scorer = other_team.top_scorer;
    this->games_counter = other_team.games_counter;
    this->goalKeepersCtr = other_team.goalKeepersCtr;
    this->players_by_id = other_team.players_by_id;
    this->players_by_goals = other_team.players_by_goals;
}


Team& Team::operator= (const Team& other_team)
{
    if(this == &other_team)
    {
        return *this;
    }

    this->TeamId = other_team.TeamId;
    this->total_points = other_team.total_points;
    this->players_num = other_team.players_num;
    this->has_goalKeeper = other_team.has_goalKeeper;
    this->total_cards = other_team.total_cards;
    this->total_goals = other_team.total_goals;
    this->games_counter = other_team.games_counter;
    this->goalKeepersCtr = other_team.goalKeepersCtr;
    this->top_scorer = other_team.top_scorer;

    return *this;
}


bool Team::operator<(const Team& other_team) const
{
    if(this->TeamId < other_team.TeamId)
    {
        return true;
    }

    return false;
}


bool Team::operator<(const Team& other_team) const
{
    if(this->TeamId < other_team.TeamId)
    {
        return true;
    }

    return false;
}


bool Team::operator==(const Team& other_team) const
{
    if(this->TeamId == other_team.TeamId)
    {
        return true;
    }
    return false;
}


int Team::getTeamId()
{
    return this->TeamId;
}

int Team::getTotalCards()
{
    return this->total_cards;
}

int Team::getTotalGoals()
{
    return this->total_goals;
}

int Team::getTotalPoints()
{
    return this->total_points;
}

int Team::getScore()
{
    return this->total_points + this->total_goals - this->total_cards;
}

int Team::getNumOfPlayers()
{
    return this->players_num;
}

int Team::getGoalkeepersCtr()
{
    return this->goalKeepersCtr;
}

Player* Team::getTopScorer()
{
    return this->top_scorer;
}

int Team::getGamesCounter()
{
   return this->games_counter;
}





void Team::setPoints(int points_added)
{
    this->total_points = this->total_points + points_added;
}

void Team::setPlayerNum(int new_player_num)
{
    this->players_num = new_player_num;
}

void Team::setTotalGoals(int goals_added)
{
    this->total_goals = goals_added;
}

void Team::setTotalCards(int cards_added)
{
    this->total_cards = cards_added;
}

void Team::setPoints(int points_added)
{
    this->total_points += points_added;
}

void Team::setTopScorer(Player* new_top_scorer)
{
    this->top_scorer = new_top_scorer;
}

void Team::setGoalkeeperCtr(int goalkeepers)
{
    this->goalKeepersCtr = goalkeepers;
}


void Team::bumpGamesCounter()
{
   this->games_counter++;
}


bool Team::canParticipate()
{
    return this->has_goalKeeper;
}

void Team::insertPlayer(Player* player_by_goals, Player* player_by_id)
{
    if(players_num == 0)
    {
        this->players_by_id = new AVLtree<Player>();
        this->players_by_goals = new AVLtree<Player>();
    }

    this->players_num++;
    this->total_goals += player_by_id->getGoals();
    this->total_cards += player_by_id->getCards();
    
    if(!(this->has_goalKeeper) && player_by_id->getGoalkeeper())
    {
        this->has_goalKeeper == true;
    }
    
    if(player_by_id->getGoals() > this->top_scorer->getGoals())
    {
        this->top_scorer = player_by_id;
    }

    this->players_by_id->Insert(player_by_id);
    this->players_by_goals->Insert(player_by_goals);

}

void Team::removePlayer(Player* player_to_remove)
{
    this->players_num--;

    if(players_num == 0)
    {
        this->players_by_id->ClearTreeKeepHead(this->players_by_id->getRoot());
        this->players_by_goals->ClearTreeKeepHead(this->players_by_goals->getRoot());

        delete this->players_by_id;
        delete this->players_by_goals;
        
        this->players_by_id = nullptr;
        this->players_by_goals = nullptr;

        this->top_scorer = nullptr;

        return;
    }

    Player* tmp_player = new Player(player_to_remove->getID(), nullptr);
    Player* player_to_remove_id = this->getPlayersById()->Find(*tmp_player);
    tmp_player->setSortingType(GOALS);
    Player* player_to_remove_goals = this->getPlayersById()->Find(*tmp_player);

    player_to_remove_id->setPlayerByGoal(nullptr);
    player_to_remove_id->setPlayerById(nullptr);
    player_to_remove_goals->setPlayerByGoal(nullptr);
    player_to_remove_goals->setPlayerById(nullptr);

    player_to_remove_id->setTeam(nullptr);
    player_to_remove_goals->setTeam(nullptr);

    this->players_by_id->DeleteActiveNode(player_to_remove_id);
    //player_to_remove->setSortingType(GOALS);
    this->players_by_goals->DeleteActiveNode(player_to_remove_goals);

    this->top_scorer = this->players_by_goals->MaxElementInfo();
    
}


   void Team::setPlayersById(AVLtree<Player>* players_by_id)
   {
        this->players_by_id = players_by_id;
   }

    
   void Team::setPlayersByGoals(AVLtree<Player>* players_by_goals)
   {
        this->players_by_goals = players_by_goals;
   }
