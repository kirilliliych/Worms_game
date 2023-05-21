#pragma once


#include <cassert>
#include "abstract_node.hpp"
#include "sfmlwrap/clock.hpp"
#include "sfmlwrap/text.hpp"


class FPSCounter : public AbstractNode
{
public:

    FPSCounter(AbstractNode *parent, const Rect<int> &area)
      : AbstractNode(parent, area),
        FPS_sum_(0),
        frames_passed_(0)
    {
        assert(fps_text_.font_load_from_file("./worms_game/fonts/CamingoMono-Regular.ttf"));
        fps_text_.set_character_size(24);
        fps_text_.set_fill_color(colors::WHITE);
        fps_text_.set_position(area_.left_x(), area_.top_y());
    }

    void render_self(Surface *surface, const Point2d<int> &camera_offset)
    {
        assert(surface != nullptr);

        FPS_sum_ += 1 / clock_.get_elapsed_time().as_seconds();
        ++frames_passed_;

        if (delay_clock_.get_elapsed_time().as_seconds() > FPS_DELAY)
        {
            update_text_(FPS_sum_ / frames_passed_);

            delay_clock_.restart();

            FPS_sum_ = 0;
            frames_passed_ = 0;
        }

        clock_.restart();

        surface->draw_text(fps_text_);
    }

private:

    void update_text_(float new_value)
    {
        const uint32_t MAX_FPS_STR_SIZE = 16;
        const uint32_t FPS_STR_FPS_VALUE_POS = 5;
        char text_str[MAX_FPS_STR_SIZE] = "FPS: 0000.00\n";
        sprintf(text_str + FPS_STR_FPS_VALUE_POS, "%.2lf\n", new_value);

        fps_text_.set_string(text_str);
    }

    static constexpr float FPS_DELAY = 0.2f;

    Text fps_text_;

    Clock clock_;
    Clock delay_clock_;

    float FPS_sum_;
    int frames_passed_;
};