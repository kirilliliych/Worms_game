#pragma once


#include "abstract_node.hpp"
#include "game.hpp"
#include "sfmlwrap/events/event.hpp"
#include <SFML/Window/Keyboard.hpp>


class Crosshair : public AbstractNode
{
public:

    Crosshair(AbstractNode *parent, int width, int height, int spin_radius, const std::string &image_file_name,
              const Rect<int> &texture_area = Rect<int>)
      : AbstractNode(parent, {width, height, {0, 0}}),
        OX_angle_(0),
        spin_radius_(spin_radius)
    {}

    void render_self(Surface *surface, const Point2d<int> &camera_offset) override
    {
        assert(surface != nullptr);
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
                            OX_angle_ -= 100.f * Game::game->time_delta.count();
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
                            OX_angle_ += 100.f * Game::game->time_delta.count();
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


            default:
            {
                break;
            }
        }

        return result;
    }

private:

    float OX_angle_;
    int spin_radius_;
};
