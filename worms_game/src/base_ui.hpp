#pragma once


#include "abstract_node.hpp"


class BaseUI : public AbstractNode
{
public:

    BaseUI(AbstractNode *parent, const Rect<int> &area, int max_value, uint32_t color)
      : AbstractNode(parent, area),
        max_value_(max_value),
        prev_value_(max_value),
        color_(color)
    {
        texture_->fill_with_color(color);
    }
 
    // void render_self(Surface *surface, const Point2d<int> &camera_offset) override
    // {
    //     assert(surface != nullptr);

    //     int value = character_parent_->get_hp();
    //     if (value > 0)
    //     {
    //         update_hp_texture_(value);
    //         prev_value_ = value;
 
    //         AbstractNode::render_self(surface, camera_offset);
    //     }
    // }
 
    // bool handle_event(const Event &event) override
    // {
    //     bool result = false;

    //     switch (event.get_type())
    //     {
    //         case EventType::TIME_PASSED:
    //         {
    //             area_.set_left_top({character_parent_->get_area().left_top().x(),
    //                                 character_parent_->get_area().left_top().y() - 10});

    //             if (children_handle_event(event))
    //             {
    //                 result = true;
    //             }

    //             break;
    //         }

    //         default:
    //         {
    //             if (children_handle_event(event))
    //             {
    //                 result = true;
    //             }

    //             break;
    //         }
    //     }

    //     return result;
    // }
 
protected:
 
    void update_value_texture_(int new_value)
    {
        int delta = new_value - prev_value_;
        if (delta == 0)
        {
            return;
        }

        int width  = area_.width();
        int height = area_.height();

        int new_width = static_cast<float> (new_value) / max_value_ * width; 
        std::vector<uint32_t> pixels(width * height, 0);
        for (int cur_width = 0; cur_width < new_width; ++cur_width)
        {
            for (int cur_height = 0; cur_height < height; ++cur_height)
            {
                pixels[cur_height * width + cur_width] = color_;
            }
        }

        texture_->update(reinterpret_cast<const uint8_t *> (pixels.data()), width, height, 0, 0);
    }

protected:

    int max_value_;
    int prev_value_;
    uint32_t color_;
};