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

        velocity_.set_x(80.f * cosf((static_cast<float> (std::rand()) / static_cast<float> (RAND_MAX))) * 2.0f * 3.14159f);
        velocity_.set_y(80.f * sinf((static_cast<float> (std::rand()) / static_cast<float> (RAND_MAX))) * 2.0f * 3.14159f);
    }

    Debris(AbstractNode *parent, const Rect<int> &area,
           const std::string &texture_file_name, const Rect<int> &texture_area = Rect<int>())
      : PhysicsObject(parent, area, {0, 0}, {0, 0}, DEFAULT_FRICTION, 8,
                      texture_file_name, texture_area)
    {
        type_ = PhysicsEntity::DEBRIS;

        velocity_.set_x(80.f * cosf((static_cast<float> (std::rand()) / static_cast<float> (RAND_MAX))) * 2.0f * 3.14159f);
        velocity_.set_y(80.f * sinf((static_cast<float> (std::rand()) / static_cast<float> (RAND_MAX))) * 2.0f * 3.14159f);
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
                if (PhysicsObject::handle_event(event))
                {
                    result = true;
                }

                for (uint32_t child_index = 0; child_index < children_.size(); ++child_index)
                {
                    if (children_[child_index]->handle_event(event))
                    {
                        result = true;
                    }
                }
                // result = children_handle_event(event);

                break;
            }

            default:
            {
                for (uint32_t child_index = 0; child_index < children_.size(); ++child_index)
                {
                    if (children_[child_index]->handle_event(event))
                    {
                        result = true;
                    }
                }
                // result = children_handle_event(event);
            }
        }

        return result;
    }

private:

    static constexpr float DEFAULT_FRICTION = 0.8;
};