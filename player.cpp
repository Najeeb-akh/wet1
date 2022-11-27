#include "Team.h"
#include "Player.h"


Player::Player(int playerId, Team* team, int games_played, int goals,
                                        int games_before_joining ,int intial_team_games, int cards, bool goalkeeper, SortByInfo SortType)
                                        : playerId(playerId), team(team), games_played(games_played), goals(goals), games_before_joining(games_before_joining) ,intial_team_games(intial_team_games),cards(cards), goalkeeper(goalkeeper),
                                                                sortType(sortType){}

Player& Player::operator=(const Player& other_player)
{
    if(this == &other_player)
    {
        return *this;
    }

    this->playerId = other_player.playerId;
    this->team = other_player.team;
    this->games_played = other_player.games_played;
    this->goals = other_player.goals;
    this->games_before_joining = games_before_joining;
    this->intial_team_games = intial_team_games;
    this->cards = other_player.cards;
    this->goalkeeper = other_player.goalkeeper;
    this->sortType = other_player.sortType;
    
    return *this;

}


/// the operator < is taking into considerations the requests in the 
//      complicated functions   
bool Player::operator<(const Player& other_player) const
{

    if(this->sortType == PLAYER_ID)
    {
        if(this->playerId > other_player.playerId)
        {
            return false;
        }
        else
        {
            return true;
        }

    }

    if(this->sortType == GOALS)
    {
        if(this->goals> other_player.goals)
        {
            return false;
        }
        else if(this->goals > other_player.goals)
        {
            return true;
        }

        else // both players have the same goals scored
        {
            if(this->cards > other_player.cards)
            {
                return false;
            }
            else if(this->playerId > other_player.playerId)
            {
                return true;
            }
            
        }

    }
}

bool Player::operator==(const Player& other_player)const
{
    return (this->playerId == other_player.playerId);
}

bool Player::operator>(const Player& other_player) const
{
    return !(this->operator<(other_player)) && !(this->operator==(other_player));
}

Team* Player::getPlayersTeam() const
{
    return this->team;
}

int Player::getID() const
{
    return this->playerId;
}

int Player::getGoals() const
{
    return this->goals;
}
int Player::getGamesPlayed() const
{
    return this->games_played;
}

int Player::getGamesPlayedBeforeJoin() const
{
    return this->games_before_joining;
}

int Player::getInitialGames() const
{
    return this->intial_team_games;
}

int Player::getCards() const
{
    return this->playerId;
}

bool Player::getGoalkeeper() const
{
    return this->goalkeeper;
}

void Player::setcards(int cards_added)
{
    this->cards = cards_added;
}

void Player::setGoals(int goals_added)
{
    this->goals = goals_added;
}

void Player::updateGamesPlayed()
{
    this->games_played++;
}

void Player::updateGoalkeeper()
{
    if(this->goalkeeper == true)
    {
        this->goalkeeper = false;
    }
    else
    {
        this->goalkeeper = true;
    }
}

void Player::setTeam(Team* new_team)
{
    this->team = new_team;

}

void Player::setSortingType(SortByInfo type)
{
    this->sortType = type;
}





/////// add new set functions ///////////