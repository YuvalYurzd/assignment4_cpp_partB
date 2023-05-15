#ifndef header9
#define header9
#include <iostream>
#include "Team.hpp"

class Team2 : public Team
{
    public:
        // Constructor
        Team2(Character *leader) : Team(leader) {}

        // Overridden functions
        void attack(Team *enemy_team) override;
        Character* find_closest_enemy(Team *enemy_team) override;
        void print() override;
};

#endif