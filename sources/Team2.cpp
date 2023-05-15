#include "Team2.hpp"

// Team2::~Team2()
// {
//     for(size_t i = 0; i < this->warriors.size(); i++)
//     {
//         delete this->warriors.at(i);
//     }
// }

void Team2::attack(Team *enemy_team)
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
    Character *target = this->find_closest_enemy(enemy_team);
    if(enemy_team->stillAlive() == 0)
        return;
    for(size_t i = 0; i < this->warriors.size(); i++)
    {
        if(enemy_team->stillAlive() == 0)
            return;
        if(target->isAlive() == false)
            target = find_closest_enemy(enemy_team);
        if(this->warriors.at(i)->type == 1)
        {
            Cowboy *c = static_cast<Cowboy *>(this->warriors.at(i));
            if(c->isAlive() == false)
                continue;
            if(c->hasboolets() == false)
                c->reload();
            else
                c->shoot(target);
        }
        if(this->warriors.at(i)->type == 2)
        {
            Ninja *c = static_cast<Ninja *>(this->warriors.at(i));
            if(c->isAlive() == false)
                continue;
            if(c->distance(target) <= 1)
                c->slash(target);
            else
                c->move(target);
        }
    }
}
Character* Team2::find_closest_enemy(Team *enemy_team)
{
    double min_dist_from_lead = __DBL_MAX__;
    for (size_t i = 0; i < enemy_team->warriors.size(); i++)
    {
        if (enemy_team->warriors.at(i)->isAlive() == true)
        {
            if (enemy_team->warriors.at(i)->distance(this->leader) < min_dist_from_lead)
                min_dist_from_lead = enemy_team->warriors.at(i)->distance(this->leader);
        }
    }
    for (size_t i = 0; i < enemy_team->warriors.size(); i++)
    {
        if (enemy_team->warriors.at(i)->isAlive() == true)
        {
            if (enemy_team->warriors.at(i)->distance(this->leader) == min_dist_from_lead)
                return enemy_team->warriors.at(i);
        }
    }
    return nullptr;
}

void Team2::print()
{
    for(size_t i =0; i < this->warriors.size(); i++)
    {
        std::cout << this->warriors.at(i)->print() << std::endl;
    }
}
