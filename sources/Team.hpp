#ifndef header8
#define header8
#include <iostream>
#include <cmath>
#include "Cowboy.hpp"
#include "OldNinja.hpp"
#include "YoungNinja.hpp"
#include "TrainedNinja.hpp"
#include "vector"

class Team
{
    public:
        std::vector<Character*> warriors;
        Character *leader;

        virtual Character* find_closest_enemy(Team *enemy_team);
        Team(Character *leader);
        // Destructor.
        virtual ~Team () {
            for (size_t i = 0; i < warriors.size(); i++) { delete warriors.at(i); }
        }
        // ~Team();
        void add(Character *chr);
        virtual void attack(Team *enemy_team);
        int stillAlive();
        virtual void print();
};
#endif