#pragma once


#include <cmath>
#include <cstdint>
#include "abstract_node.hpp"
#include "game.hpp"
#include "maths.hpp"
#include "sfmlwrap/events/event.hpp"


class Crosshair : public AbstractNode
{
public:

    Crosshair(AbstractNode *parent, const Rect<int> &area, int spin_radius, uint32_t color)
      : AbstractNode(parent, area),
        OX_angle_(-math_consts::PI),
        spin_radius_(spin_radius)
    {
        form_crosshair_texture_(color);
    }

    float get_angle() const
    {
        return OX_angle_;
    }

    bool is_right_semicircle() const
    {
        return (-math_consts::HALF_PI <= OX_angle_) && (OX_angle_ <= math_consts::HALF_PI);
    }

    void render_self(Surface *surface, const Point2d<int> &camera_offset) override
    {
        assert(surface != nullptr);

        if (Game::game->is_under_control(parent_))
        {
            AbstractNode::render_self(surface, camera_offset);
        }
    }

    bool handle_event(const Event &event) override
    {
        bool result = false;

        switch (event.get_type())
        {
            case EventType::KEY_PRESSED:
            {
                switch (event.kedata_.key_code)
                {
                    case KeyboardKey::Left:
                    {
                        if (Game::game->is_under_control(parent_))
                        {
                            OX_angle_ -= 15.f * Game::game->time_delta.count();
                            if (OX_angle_ < -math_consts::PI)
                            {
                                OX_angle_ += math_consts::PI * 2;
                            }
                        }

                        break;
                    }

                    case KeyboardKey::Right:
                    {
                        if (Game::game->is_under_control(parent_))
                        {
                            OX_angle_ += 15.f * Game::game->time_delta.count();
                            if (OX_angle_ > math_consts::PI)
                            {
                                OX_angle_ -= math_consts::PI * 2;
                            }
                        }

                        break;
                    }

                    default:
                    {
                        break;
                    }
                }

                break;
            }

            case EventType::TIME_PASSED:
            {
                area_.set_left_top(parent_->get_area().center() +
                                   Point2d<int>(static_cast<int> (cosf(OX_angle_) * spin_radius_),
                                                static_cast<int> (sinf(OX_angle_) * spin_radius_)));

                break;
            }


            default:
            {
                break;
            }
        }

        return result;
    }

private:

    void form_crosshair_texture_(uint32_t color)
    {
        uint32_t width  = texture_->get_width();
        uint32_t height = texture_->get_height();
        uint32_t min_dim = std::min(width, height);
        uint32_t thickness = min_dim / 8 + 1;
        uint32_t texture_horizontal_line_top_edge_y = height / 2 - thickness / 2;
        uint32_t texture_vertical_line_left_edge_x  = width  / 2 - thickness / 2;
        vector<uint32_t> pixels(width * height, 0);
        for (uint32_t cur_width = 0; cur_width < width; ++cur_width)
        {
            for (uint32_t cur_thickness = 0; cur_thickness < thickness; ++cur_thickness)
            {
                pixels[(texture_horizontal_line_top_edge_y + cur_thickness) * width + cur_width] = color;
            }
        }
        for (uint32_t cur_height = 0; cur_height < height; ++cur_height)
        {
            for (uint32_t cur_thickness = 0; cur_thickness < thickness; ++cur_thickness)
            {
                pixels[cur_height * width + texture_vertical_line_left_edge_x + cur_thickness] = color;
            }
        }

        texture_->update(reinterpret_cast<const uint8_t *> (pixels.data()), width, height, 0, 0);
    }

private:

    float OX_angle_;
    int spin_radius_;
};
