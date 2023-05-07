#pragma once


#include <cmath>
#include <cstdint>
#include "abstract_node.hpp"
#include "game.hpp"
#include "sfmlwrap/events/event.hpp"


class Crosshair : public AbstractNode
{
public:

    Crosshair(AbstractNode *parent, const Rect<int> &area, int spin_radius)
      : AbstractNode(parent, area),
        OX_angle_(0),
        spin_radius_(spin_radius)
    {
        form_crosshair_texture_(0xff0000ff);
    }

    float get_angle() const
    {
        return OX_angle_;
    }

    void render_self(Surface *surface, const Point2d<int> &camera_offset) override
    {
        assert(surface != nullptr);

        if (parent_ == reinterpret_cast<const AbstractNode *> (Game::game->get_character_under_control()))
        {
            // Sprite self_sprite(*texture_, area_.left_top() - camera_offset);
            // surface->draw_sprite(self_sprite);

            Point2d<int> crosshair_offset{static_cast<int> (cosf(OX_angle_) * spin_radius_),
                                          static_cast<int> (sinf(OX_angle_) * spin_radius_)
                                         };
            Sprite self_sprite(*texture_, area_.left_top() + crosshair_offset - camera_offset);
            surface->draw_sprite(self_sprite);
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
                        if (parent_ == reinterpret_cast<const AbstractNode *> (Game::game->get_character_under_control()))
                        {
                            OX_angle_ -= 15.f * Game::game->time_delta.count();
                            if (OX_angle_ < -3.14159f)
                            {
                                OX_angle_ += 3.14159f * 2;
                            }
                        }

                        break;
                    }

                    case KeyboardKey::Right:
                    {
                        if (parent_ == reinterpret_cast<const AbstractNode *> (Game::game->get_character_under_control()))
                        {
                            OX_angle_ += 15.f * Game::game->time_delta.count();
                            if (OX_angle_ > 3.14159f)
                            {
                                OX_angle_ -= 3.14159f * 2;
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
                area_.set_left_top(parent_->get_area().center());

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
        std::vector<uint32_t> pixels(width * height, 0);
        uint32_t *pixels_data = pixels.data();
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

        texture_->update(reinterpret_cast<const uint8_t *> (pixels_data), width, height, 0, 0);
    }

private:

    float OX_angle_;
    int spin_radius_;
};
