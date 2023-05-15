#include "Character.hpp"
#include <iostream>
#include <cmath>

using namespace std;

Character::Character()
{
}
Character::Character(const Point &pnt, int _hp, std::string name)
{
    this->location = pnt;
    this->hp = _hp;
    this->name = name;
    this->in_team = false;
    this->type = 0;
}



bool Character::isAlive()
{
    return (this->hp > 0);
}
double Character::distance(Character *chr)
{
    return this->getLocation().distance(chr->getLocation());
}
void Character::hit(int damage)
{
    if(damage < 0)
        throw std::invalid_argument("cannot deal negative damage");
    this->hp -= damage;
}
std::string Character::getName()
{
    return this->name;
}
Point Character::getLocation()
{
    return this->location;
}
std::string Character::print()
{
    return "";
}