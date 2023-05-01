#pragma once


#include <cstdlib>
#include <vector>
#include "abstract_node.hpp"
#include "character.hpp"
#include "point2d.hpp"


namespace character_consts
{
    const uint32_t SPAWN_Y_COORD = 100;
}


class Team : public AbstractNode
{
public:

    Team(AbstractNode *parent, uint32_t members_quantity, int spawn_position_center_x, int variance,
         uint32_t character_pixel_width, uint32_t character_pixel_height)
      : AbstractNode(parent, {0, 0, {0, 0}}),
        members_quantity_(members_quantity),
        members_(members_quantity),
        priority_(members_quantity, 0)
    {
        std::vector<int> spawn_positions_x = randomize_positions_(spawn_position_center_x,
                                                                  variance);

        for (uint32_t member_index = 0; member_index < members_quantity; ++member_index)
        {
            members_[member_index] = new Character(this, {static_cast<int> (character_pixel_width),
                                                                static_cast<int> (character_pixel_height),
                                                        {spawn_positions_x[member_index], 
                                                            character_consts::SPAWN_Y_COORD}},
                                                   "standing.png");
        }
    }

    ~Team()
    {
        auto members_iterator = members_.cbegin();
        auto members_end = members_.cend();
        while (members_iterator != members_end)
        {
            delete *members_iterator;

            ++members_iterator;
        }
    }

private:

    std::vector<int> randomize_positions_(int spawn_position_center_x, int variance)
    {
        std::vector<int> result(members_quantity_);
        for (uint32_t i = 0; i < members_quantity_; ++i)
        {
            result[i] = std::rand() % variance - variance / 2 + spawn_position_center_x;
        }

        return result;
    }

    void form_priority_()
    {
        uint32_t left_to_generate = members_quantity_;
        while (left_to_generate > 0)
        {
            uint32_t index = std::rand() % left_to_generate;
            if (priority_[index] == 0)
            {
                priority_[index] = members_quantity_ + 1 - left_to_generate;
                --left_to_generate;
            }
        }
    }

    uint32_t members_quantity_;
    std::vector<Character *> members_;
    std::vector<uint32_t> priority_;
};
