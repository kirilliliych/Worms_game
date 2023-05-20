#pragma once


#include "abstract_node.hpp"
#include "colors.hpp"
#include "game.hpp"
#include "sfmlwrap/clock.hpp"
#include "sfmlwrap/text.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Text.hpp>


class TurnTimeCounter : public AbstractNode
{
public:

    TurnTimeCounter(AbstractNode *parent, const Rect<int> &area, float turn_time_limit = TURN_TIME_LIMIT)
      : AbstractNode(parent, area),
        time_counter_text_(),
        turn_time_left_(turn_time_limit),
        prev_turn_time_left_(turn_time_limit),
        turn_time_limit_(turn_time_limit),
        is_freezed_(false)
    {
        time_counter_text_.font_load_from_file("./worms_game/fonts/CamingoMono-Regular.ttf");
        time_counter_text_.set_character_size(36);
        time_counter_text_.set_fill_color(colors::WHITE);
    }

    float get_time_left() const
    {
        return turn_time_left_;
    }

    void restart()
    {
        prev_turn_time_left_ = turn_time_limit_;
        turn_time_left_      = turn_time_limit_;
    }

    bool expired() const
    {
        return turn_time_left_ <= 0;
    }

    void freeze()
    {
        is_freezed_ = true;
    }

    void resume()
    {
        is_freezed_ = false;
    }

    void render_self(Surface *surface, const Point2d<int> &camera_offset) override
    {
        assert(surface != nullptr);

        int prev_floor = std::floor(prev_turn_time_left_); 
        int cur_floor  = std::floor(turn_time_left_);
        if (prev_floor != cur_floor)
        {
            update_text_(cur_floor);
        }
        surface->draw_text(time_counter_text_);
    }

    bool handle_event(const Event &event) override
    {
        bool result = false;

        switch (event.get_type())
        {
            case EventType::TIME_PASSED:
            {
                if (!is_freezed_)
                {
                    prev_turn_time_left_ = turn_time_left_;
                    turn_time_left_ -= Game::game->time_delta.count();
                }

                if (children_handle_event(event))
                {
                    result = true;
                }

                break;
            }

            default:
            {
                if (children_handle_event(event))
                {
                    result = true;
                }

                break;
            }
        }

        return result;
    }

private:

    void update_text_(int new_value)
    {
        const uint32_t MAX_STR_SIZE = 16;
        char text_str[MAX_STR_SIZE] = {};
        sprintf(text_str, "%d\n", new_value);

        time_counter_text_.set_string(text_str);
    }

private:

    static constexpr float TURN_TIME_LIMIT = 15.f;

    Text time_counter_text_;

    float turn_time_left_;
    float prev_turn_time_left_;
    float turn_time_limit_;

    bool is_freezed_;
};
