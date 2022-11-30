#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "AVL_NODE.h"
class Team;

typedef enum{
    PLAYER_ID = 0,
    GOALS = 1,
    CARDS = 2
} SortByInfo;

class Player
{
    int playerId;
    Team* team;
    int games_played;
    int goals;
    int games_before_joining;
    int intial_team_games;
    int cards;
    bool goalkeeper;
    AVLnode<Player>* player_goal_place;
    SortByInfo sortType;


    public:
        // Player: consturctor for the player class 
        Player(int playerId, Team* team, int games_played = 0, int goals=0
                                        , int games_before_joining = 0, int intial_team_games=0 ,int cards = 0, bool goalkeeper = 0,AVLnode<Player>* player_goal_place=nullptr, SortByInfo SortType = PLAYER_ID);
        // a default destructor for the player class
        ~Player() = default;

        // a defalut copy constructor for the player class
        Player(const Player& player) = default;
        
        //operator= : an assignment operator between two players, the local player and another 
        Player& operator=(const Player& other_player);

        /** operator<: checks whether the local player is smaller than the given other player according to 
        *             the instructions of the HW 
        * 
        * @param other - target player for the local player to be compared with
        * 
        * @return true, in case the local player is smaller than the given player.
        *         Otherwise, false.
        */     
        bool operator<(const Player& other_player) const;
        
        /** operator==: checks the equality between the local player and the given Player according to 
         *             the instructions of the HW 
         * 
         * @param other - target player for the local player to be compared with
         * 
         * @return true, in case the local player is equal to the given player.
         *         Otherwise, false.
         */
        bool operator==(const Player& other_player) const;

        /** operator>: checks whether the local player is bigger than the given other player according to 
         *             the instructions of the HW 
         * 
         * @param other - target player for the local player to be compared with
         * 
         * @return true, in case the local player is bigger than the given employee.
         *         Otherwise, false.
         */
        bool operator>(const Player& other_player) const;
        
        /** getPlayersTeam: team getter
        *  
        *  @return pointer to the team that include the player
        */
        Team* getPlayersTeam() const;
        
        // getId: returns local player id
        int getID() const;
        
        // getGoals: return the goals of the local player
        int getGoals() const;

        int getGamesPlayed() const;

        int getGamesPlayedBeforeJoin() const;
        
        int getInitialGames() const;
    
        
        //getCards: return the cards of the local player
        int getCards() const;
        
        /** getGoalKeeper: checks whether the local player is a goalkeeper or not 
         * 
         * @return true, in case the local employee is a goalkeeper
         *         Otherwise, false.
         */
        bool getGoalkeeper() const;

        AVLnode<Player>* getPlayerByGoal() const;
        void setPlayerByGoal(AVLnode<Player>* other_player_by_goal);
        
        // setcards: set the local player cards field to the param given.
        void setcards(int cards_added);

        // setGoals: set the local player goals field to the param given.
        void setGoals(int goals_added);
        
        // setGamesPlayed: set the local player games_played field to the param given.
        void setGamesPlayed(int games_added);

        void setGamesPlayedBeforeJoin(int games_before_joining);
        
        void setInitialGames(int initial_games);

        // updateGamesPlayed: add one to the local player gameplayed field
        void updateGamesPlayed();

        // updateGoalKeeper: switch the player to be a goalkeeper if he isnt,
        //                  and if he is a goalkeeper make him a regular player and not a goalkeeper
        void updateGoalkeeper();
        
        // setTeam: set the local player team into the team given as a param
        void setTeam(Team* new_team);
        
        /**
         * setSortingType: Set the Sorting Type object
         * 
         * @type: The sorting type to set
         */
        void setSortingType(SortByInfo type);
        

};

#endif