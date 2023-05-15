#ifndef header3
#define header3
#include <iostream>
#include <cmath>

#include "Character.hpp"

class Cowboy : public Character
{
    public:
        int numofbullets;


        Cowboy();
        Cowboy(std::string name, Point location);

        std::string print();
        void shoot(Character *enemy);
        bool hasboolets();
        void reload();
        

};
#endif