#pragma once


#include <cstdlib>
#include <vector>
#include "abstract_node.hpp"
#include "base_ui.hpp"
#include "character.hpp"
#include "point2d.hpp"
#include "sfmlwrap/events/event.hpp"


namespace character_consts
{
    const uint32_t SPAWN_Y_COORD = 100;
}

class Team;

class TeamUI : public BaseUI
{
public:

    TeamUI(AbstractNode *parent, const Rect<int> &area, int max_value, uint32_t color);
    //   : BaseUI(parent, area, max_value, color),
    //     team_parent_(dynamic_cast<const Team *> (parent))
    // {
    //     assert(team_parent_ != nullptr);
    // }

    void render_self(Surface *surface, const Point2d<int> &camera_offset) override;
    // {
    //     assert(surface != nullptr);

    //     int value = team_parent_->get_hp();
    //     if (value > 0)
    //     {
    //         update_value_texture_(value);
    //         prev_value_ = value;

    //         AbstractNode::render_self(surface, camera_offset);
    //     }
    // }

    bool handle_event(const Event &event) override;

private:

    const Team *team_parent_;
};


class Team : public AbstractNode
{
public:

    Team(AbstractNode *parent, uint32_t members_quantity, int spawn_position_center_x, int variance,
         uint32_t character_pixel_width, uint32_t character_pixel_height, uint32_t color, const Rect<int> &team_hp_area)
      : AbstractNode(parent, team_hp_area),
        members_quantity_(members_quantity),
        members_(members_quantity),
        team_ui_(new TeamUI(this, team_hp_area, members_quantity * DEFAULT_HP, color)),
        sequence_(members_quantity),
        sequence_cur_character_index_(0)
    {
        std::vector<int> spawn_positions_x = randomize_positions_(spawn_position_center_x,
                                                                  variance);

        for (uint32_t member_index = 0; member_index < members_quantity; ++member_index)
        {
            members_[member_index] = new Character(this, {static_cast<int> (character_pixel_width),
                                                          static_cast<int> (character_pixel_height),
                                                          {spawn_positions_x[member_index], 
                                                           character_consts::SPAWN_Y_COORD
                                                          }
                                                         },
                                                         DEFAULT_HP,
                                                        "standing.png",
                                                        color);
        }

        form_priority_();

        texture_->fill_with_color(color);
    }

    ~Team()
    {
        // auto members_iterator = members_.cbegin();
        // auto members_end = members_.cend();
        // while (members_iterator != members_end)
        // {
        //     delete *members_iterator;

        //     ++members_iterator;
        // }

        delete team_ui_;
    }

    const Character *get_next_character()
    {
        assert(is_alive());

        const Character *result = members_[sequence_cur_character_index_];
        while (result->get_hp() < 0)
        {
            ++sequence_cur_character_index_;
            sequence_cur_character_index_ %= members_quantity_;

            result = members_[sequence_cur_character_index_];
        }
        ++sequence_cur_character_index_;
        sequence_cur_character_index_ %= members_quantity_;

        return result;
    }

    int get_hp() const
    {
        int result = 0;
        for (uint32_t member_index = 0; member_index < members_quantity_; ++member_index)
        {
            int member_hp = members_[member_index]->get_hp();
            if (member_hp > 0)
            {
                result += member_hp;
            }
        }

        return result;
    }

    bool is_alive() const
    {
        bool is_team_alive = false;
        for (uint32_t member_index = 0; member_index < members_quantity_; ++member_index)
        {
            if (members_[member_index]->is_alive())
            {
                is_team_alive = true;

                break;
            }
        }

        return is_team_alive;
    }

    void render_self(Surface *surface, const Point2d<int> &camera_offset) override
    {
        assert(surface != nullptr);
    }

    bool handle_event(const Event &event) override
    {
        bool result = false;

        switch (event.get_type())
        {
            default:
            {
                if (children_handle_event(event))
                {
                    result = true;
                }
            }
        }

        return result;
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
        std::vector<uint32_t> marks(members_quantity_);
        for (uint32_t i = 0; i < members_quantity_; ++i)
        {
            marks[i] = i;
        }
        while (left_to_generate > 0)
        {
            uint32_t index = std::rand() % left_to_generate;
            sequence_[members_quantity_ - left_to_generate] = marks[index];

            marks.erase(marks.cbegin() + index);
            --left_to_generate;
        }
    }

private:

    static constexpr int DEFAULT_HP = 100;

    uint32_t members_quantity_;
    std::vector<Character *> members_;

    TeamUI *team_ui_;

    std::vector<uint32_t> sequence_;
    uint32_t sequence_cur_character_index_;
};



inline TeamUI::TeamUI(AbstractNode *parent, const Rect<int> &area, int max_value, uint32_t color)
  : BaseUI(parent, area, max_value, color),
    team_parent_(dynamic_cast<const Team *> (parent))
{
    assert(team_parent_ != nullptr);
}

inline void TeamUI::render_self(Surface *surface, const Point2d<int> &camera_offset)
{
    assert(surface != nullptr);

    int value = team_parent_->get_hp();
    if (value > 0)
    {
        update_value_texture_(value);
        prev_value_ = value;

        AbstractNode::render_self(surface, {0, 0});
    }
}

inline bool TeamUI::handle_event(const Event &event)
{
    bool result = false;

    switch (event.get_type())
    {
        default:
        {
            if (children_handle_event(event))
            {
                result = true;
            }
        }
    }

    return result;
}
