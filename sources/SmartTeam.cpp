#include "SmartTeam.hpp"

void SmartTeam::attack(Team *enemy_team)
{
   if(this->stillAlive() == 0)
        throw std::invalid_argument("cannot attack - no players alive");
    if(enemy_team == nullptr)
        throw std::invalid_argument("cannot attack null team");
    if(enemy_team->stillAlive() == 0)
        throw std::runtime_error("cannot attack dead team");
    if(enemy_team == this)
        throw std::invalid_argument("no friendly fire");
    if (this->leader->isAlive() == false)
        this->leader = find_closest_enemy(this);
    
    for(size_t i = 0; i < this->warriors.size(); i++)
    {
        if(enemy_team->stillAlive() == 0)
            return;
        if(this->warriors.at(i)->type == 2)
        {
            Ninja *c = static_cast<Ninja *>(this->warriors.at(i));
            Character *target = find_target_for_ninja(c,enemy_team);
            if(c->isAlive() == false)
                continue;
            if(c->distance(target) <= 1)
                c->slash(target);
            else
                c->move(target);
        }
    }

    for(size_t i = 0; i < this->warriors.size(); i++)
    {
        if(enemy_team->stillAlive() == 0)
            return;
        if(this->warriors.at(i)->type == 1)
        {
            Cowboy *c = static_cast<Cowboy *>(this->warriors.at(i));
            Character *target = find_target_for_cowboy(enemy_team);
            if(c->isAlive() == false)
                continue;
            if(c->hasboolets())
                c->shoot(target);
            else
                c->reload();
        }
    }
}

void SmartTeam::print()
{
    for(size_t i = 0; i < this->warriors.size(); i++)
    {
        std::cout << this->warriors.at(i)->print() << std::endl;
    }
}

Character* SmartTeam::find_target_for_ninja(Ninja *n, Team *enemy_team)
{
    double min_dist_from_ninja = __DBL_MAX__;
    for (size_t i = 0; i < enemy_team->warriors.size(); i++)
    {
        if (enemy_team->warriors.at(i)->isAlive() == true)
        {
            if (enemy_team->warriors.at(i)->distance(n) < min_dist_from_ninja)
                min_dist_from_ninja = enemy_team->warriors.at(i)->distance(this->leader);
        }
    }
    for (size_t i = 0; i < enemy_team->warriors.size(); i++)
    {
        if (enemy_team->warriors.at(i)->isAlive() == true)
        {
            if (enemy_team->warriors.at(i)->distance(n) == min_dist_from_ninja)
                return enemy_team->warriors.at(i);
        }
    }
    
    return nullptr;
}

Character* SmartTeam::find_target_for_cowboy(Team *enemy_team)
{
    int lowest_hp = 150;
    for (size_t i = 0; i < enemy_team->warriors.size(); i++)
    {
        if (enemy_team->warriors.at(i)->isAlive() == true)
        {
            if (enemy_team->warriors.at(i)->hp < lowest_hp)
                lowest_hp = enemy_team->warriors.at(i)->hp;
        }
    }
    for (size_t i = 0; i < enemy_team->warriors.size(); i++)
    {
        if (enemy_team->warriors.at(i)->isAlive() == true)
        {
            if (enemy_team->warriors.at(i)->hp == lowest_hp)
                return enemy_team->warriors.at(i);
        }
    }
    return nullptr;
}
