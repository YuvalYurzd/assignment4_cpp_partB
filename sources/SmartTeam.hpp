#ifndef header10
#define header10
#include <iostream>
#include "Team.hpp"

class SmartTeam : public Team
{
    public:
        // Constructor
        SmartTeam(Character *leader) : Team(leader) {}

        // Overridden functions
        void attack(Team *enemy_team) override;
        void print() override;
        Character* find_target_for_ninja(Ninja *n, Team *enemy_team);
        Character* find_target_for_cowboy(Team *enemy_team);

};

#endif