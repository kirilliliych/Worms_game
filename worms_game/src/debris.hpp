#pragma once


#include "abstract_node.hpp"
#include "physics_entity.hpp"
#include "physics_object.hpp"


class Debris : public PhysicsObject
{
public:

    Debris(AbstractNode *parent, const Rect<int> &area)
      : PhysicsObject(parent, area, {0, 0}, {0, 0}, DEFAULT_FRICTION, 8)
    {
        type_ = PhysicsEntity::DEBRIS;

        velocity_.set_x(400.f * cosf((static_cast<float> (std::rand()) / static_cast<float> (RAND_MAX))) * 2.0f * 3.14159f);
        velocity_.set_y(400.f * sinf((static_cast<float> (std::rand()) / static_cast<float> (RAND_MAX))) * 2.0f * 3.14159f);
    }

    Debris(AbstractNode *parent, const Rect<int> &area,
           const std::string &texture_file_name)
      : PhysicsObject(parent, area, {0, 0}, {0, 0}, DEFAULT_FRICTION, 8,
                      texture_file_name)
    {
        type_ = PhysicsEntity::DEBRIS;

        velocity_.set_x(400.f * cosf((static_cast<float> (std::rand()) / static_cast<float> (RAND_MAX)) * 2.0f * 3.14159f));
        velocity_.set_y(400.f * sinf((static_cast<float> (std::rand()) / static_cast<float> (RAND_MAX)) * 2.0f * 3.14159f));
    }

    void on_bounce_death(const Point2d<int> &death_position) override
    {}

    bool handle_event(const Event &event) override
    {
        bool result = false;

        switch (event.get_type())
        {
            case EventType::TIME_PASSED:
            {
                // if (PhysicsObject::handle_event(event))
                // {
                //     result = true;
                // }

                handle_physics();

                if (children_handle_event(event))
                {
                    result = true;
                }

                break;
            }

            case EventType::STABILITY_EVENT:
            {
                if (!is_stable_)
                {
                    result = true;
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
            }
        }

        return result;
    }

private:

    static constexpr float DEFAULT_FRICTION = 0.8;
};