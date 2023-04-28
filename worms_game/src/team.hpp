#ifndef TEAM_HPP
#define TEAM_HPP


#include <vector>
#include "character.hpp"
#include "point2d.hpp"


class Team
{
public:

    Team(uint32_t quantity)
    {

    }

private:

    void form_priority_()
    {
        
    }


    uint32_t characters_quantity_;
    std::vector<Character> characters_;
    std::vector<uint32_t> priority_;
};


#endif
