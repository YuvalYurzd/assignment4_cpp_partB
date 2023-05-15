#ifndef header4
#define header4
#include <iostream>
#include <cmath>

#include "Character.hpp"

class Ninja : public Character
{
    public:
        int speed;

        Ninja();
        Ninja(std::string name, Point location, int _hp, int speed);

        std::string print();
        void move(Character *enemy);
        void slash(Character *enemy);
    
};
#endif