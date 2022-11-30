#ifndef TEAM_H
#define TEAM_H

#include "AVL_TREEE.h"
class Player;

class Team
{
    int TeamId;
    AVLtree<Player>* players_by_goals;
    AVLtree<Player>* players_by_id;
    int total_points;
    int players_num;
    bool has_goalKeeper;
    int total_goals;
    int total_cards;
    int games_counter;
    int goalKeepersCtr;
    Player* top_scorer;

    public:
    /**
     * @brief Construct a new Team object
     * 
     * @param TeamId 
     * @param points 
     */
        // Team(int TeamId, int points);

        Team(int TeamId, int points= 0, int player_num = 0, bool has_goalkeeper=false,
                         int total_goals = 0, int total_cards = 0, int games_counter = 0, int goalKeeperCtr = 0,
                        Player* top_scorer = nullptr, AVLtree<Player>* players_by_id = nullptr, AVLtree<Player>* players_by_goals = nullptr);
        /**
         * @brief Destroy the Team object
         * 
         */
        ~Team() = default;

        /**
         * @brief copy constructor for Team
         * 
         * @param team 
         */
        Team(const Team& other_team);



        //--------------operators--------------



        /**
         * @brief assignment operator for team
         * 
         * @param other_team 
         * @return Team& 
         */
        Team& operator=(const Team& other_team);

        /**
         * @brief operator < for team
         * 
         * @param other_team 
         * @return true 
         * @return false 
         */
        bool operator<(const Team& other_team) const;

        /**
         * @brief operator > for team
         * 
         * @param other_team 
         * @return true 
         * @return false 
         */
        bool operator>(const Team& other_team) const;

        //bool operator<=(const Team& other_team);
        //bool operator>=(const Team& other_team);

        /**
         * @brief operator == for Team
         * 
         * @param other_team 
         * @return true 
         * @return false 
         */
        bool operator==(const Team& other_team) const;



        //---------get functions----------


        /**
         * @brief Get the Team Id
         * 
         * @return int 
         */
        int getTeamId();
        

        /**
         * @brief Get the Total Goals in Team
         * 
         * @return int 
         */
        int getTotalGoals();


        /**
         * @brief Get the Total Cards in Team
         * 
         * @return int 
         */
        int getTotalCards();

        /**
         * @brief Get the Total Points in Team
         * 
         * @return int 
         */
        int getTotalPoints();

    
        /**
         * @brief Get the Num Of Players in Team
         * 
         * @return int 
         */
        int getNumOfPlayers();


        /**
         * @brief Get the Goalkeepers Ctr object
         * 
         * @return int 
         */
        int getGoalkeepersCtr();


        /**
         * @brief Get the Top Scorer 
         * 
         * @return Player* 
         */
        Player* getTopScorer();

        /**
         * @brief get the Total Cards for the team
         * 
         */
        int getGamesCounter();

        
        //-------------set functions-------------


        /**
         * @brief Add/subtract points to/from the team
         * 
         * @param points_added 
         */
        void setPoints(int points_added);


        /**
         * @brief Set the Total Players for the team
         * 
         * @param new_players_num 
         */
        void setPlayerNum(int new_players_num);


        /**
         * @brief Set the Total Goals for the team
         * 
         * @param goals_added 
         */
        void setTotalGoals(int goals_added);

        

        /**
         * @brief Set the Total Cards for the team
         * 
         * @param cards_added 
         */
        void setTotalCards(int cards_added);

        
        /**
         * @brief changes the 'hasGoalKeeper' to the given parameter
         * 
         * 
         */
        void setGoalKeeper(bool changed_value);

        /**
         * @brief Set the Top Scorer
         * 
         * @param new_top_scorer 
         */
        void setTopScorer(Player* new_top_scorer);


        /**
         * @brief adds the goalKeepers counter by 1
         * 
         */
        void addGoalKeepersCtr();


        /**
         * @brief subtract the goalkeepers counter by 1
         * 
         */
        void subtractGoalKeepersCtr();


        /**
         * @brief add 1 to the Total Cards for the team
         * 
         */
        void bumpGamesCounter();

        //-------------helper functions-----------


        /**
         * @brief check if the team can participate in the games
         * 
         * @return true if there is a goal keeper
         * @return false if there isnt a goal keeper
         */
        bool canParticipate();


        /**
         * @brief function to insert a player into the team
         * 
         * @param player_by_goals 
         * @param player_by_id 
         */
        void insertPlayer(Player* player_by_goals, Player* player_by_id);


        /**
         * @brief a function to remove the player from the team
         * 
         * @param player_to_remove 
         */
        void removePlayer(Player* player_to_remove);


        /**
         * @brief Get the Players tree sorted by id
         * 
         * @return AVLtree<Player>* 
         */
        AVLtree<Player>* getPlayersById();


        /**
         * @brief Get the Players tree sorted by goals > cards > id
         * 
         * @return AVLtree<Player>* 
         */
        AVLtree<Player>* getPlayersByGoals();




};

#endif