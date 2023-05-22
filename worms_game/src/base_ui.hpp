#pragma once


#include "abstract_node.hpp"
#include "stl.hpp"


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
        if (new_width <= 0)
        {
            new_width = 1;
        }
        vector<uint32_t> pixels(width * height, 0);
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