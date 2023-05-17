#include "Character.hpp"
#include <iostream>
#include <cmath>
#include "Ninja.hpp"

using namespace std;

Ninja::Ninja()
{

}

Ninja::Ninja(std::string name, Point place, int _hp, int speed) : Character(place,_hp,name), speed(speed){this->type = 2;}


void Ninja::move(Character *enemy)
{
        this->location = this->location.moveTowards(this->getLocation(),enemy->getLocation(),this->speed);
}

void Ninja::slash(Character * enemy)
{
    if(this->hp <= 0)
        throw std::runtime_error("cannot attack while dead");
    if(this == enemy)
        throw std::runtime_error("You cannot slash yourself");
    if(enemy->isAlive() == false)
        throw std::runtime_error("You cannot slash a dead target");
    if(this->getLocation().distance(enemy->getLocation()) <= 1)
    {
        enemy->hit(40);
    }
}

std::string Ninja::print()
{
    string res = "";
    res += "N (" + this->getName() + ")";
    if(this->isAlive() == true)
        res += " " + to_string(this->hp);
    res += " (" + to_string(this->getLocation().getX()) + "," + to_string(this->getLocation().getY()) + ")";
    return res;
}