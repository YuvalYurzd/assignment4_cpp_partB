#include "Team.hpp"

// Constructor for Team class
Team::Team(Character *leader)
{

    if(leader->in_team == true)
        throw std::runtime_error("leader belongs to team");
    this->leader = leader;
    this->leader->in_team = true;
    this->warriors.push_back(leader);
}


// Team::~Team()
// {
//      for(size_t i = 0; i < this->warriors.size(); i++)
//     {
//         delete this->warriors.at(i);
//     }
// }
// Add a Character to the team
void Team::add(Character *chr)
{
    if (!chr)
        return;
    if (this->warriors.size() == 10)
        throw std::runtime_error("team is full");
    if (chr->in_team == true)
        throw std::runtime_error("already belongs to a team");
    this->warriors.push_back(chr);
    chr->in_team = true;
}


// attack another team
void Team::attack(Team *enemy_team)
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
    }

    for(size_t i = 0; i < this->warriors.size(); i++)
    {
        if(enemy_team->stillAlive() == 0)
            return;
        if(target->isAlive() == false)
            target = find_closest_enemy(enemy_team);
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

Character* Team::find_closest_enemy(Team *enemy_team)
{
    double min_dist_from_lead = __DBL_MAX__;
    for (size_t i = 0; i < enemy_team->warriors.size(); i++)
    {
        if (enemy_team->warriors.at(i)->isAlive() == true && enemy_team->warriors.at(i)->type == 1)
        {
            if (enemy_team->warriors.at(i)->distance(this->leader) < min_dist_from_lead)
                min_dist_from_lead = enemy_team->warriors.at(i)->distance(this->leader);
        }
    }
    for (size_t i = 0; i < enemy_team->warriors.size(); i++)
    {
        if (enemy_team->warriors.at(i)->isAlive() == true && enemy_team->warriors.at(i)->type == 2)
        {
            if (enemy_team->warriors.at(i)->distance(this->leader) < min_dist_from_lead)
                min_dist_from_lead = enemy_team->warriors.at(i)->distance(this->leader);
        }
    }
    for (size_t i = 0; i < enemy_team->warriors.size(); i++)
    {
        if (enemy_team->warriors.at(i)->isAlive() == true && enemy_team->warriors.at(i)->type == 1)
        {
            if (enemy_team->warriors.at(i)->distance(this->leader) == min_dist_from_lead)
                return enemy_team->warriors.at(i);
        }
    }
    for (size_t i = 0; i < enemy_team->warriors.size(); i++)
    {
        if (enemy_team->warriors.at(i)->isAlive() == true && enemy_team->warriors.at(i)->type == 2)
        {
            if (enemy_team->warriors.at(i)->distance(this->leader) == min_dist_from_lead)
                return enemy_team->warriors.at(i);
        }
    }
    return nullptr;
}
int Team::stillAlive()
{
    int count = 0;
    for (size_t i = 0; i < this->warriors.size(); i++)
    {
        if (this->warriors.at(i)->isAlive() == true)
            count++;
    }
    return count;
}

void Team::print()
{
    // print all cowboys
    for(size_t i = 0; i < this->warriors.size(); i++)
    {
        if(this->warriors.at(i)->type == 1)
        {
            std::cout << this->warriors.at(i)->print() << std::endl;
        }
    }

    // print all ninjas
    for(size_t i = 0; i < this->warriors.size(); i++)
    {
        if(this->warriors.at(i)->type == 2)
        {
            std::cout << this->warriors.at(i)->print() << std::endl;
        }
    }
}
