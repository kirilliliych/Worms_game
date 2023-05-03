#ifndef PHYSICS_OBJECT_HPP
#define PHYSICS_OBJECT_HPP


#include <cmath>
#include "abstract_node.hpp"
#include "game.hpp"
#include "physics_entity.hpp"
#include "sfmlwrap/events/event.hpp"
#include "sfmlwrap/rect.hpp"
#include "vector2d.hpp"


class PhysicsObject : public AbstractNode
{
protected:

    PhysicsObject(AbstractNode *parent, const Rect<int> &area, const Vector2d<float> &velocity,
                  const Vector2d<float> &acceleration, float friction, int bounces_before_death)
      : AbstractNode(parent, area),
        type_(PhysicsEntity::UNKNOWN),
        velocity_(velocity),
        acceleration_(acceleration),
        friction_(friction),
        bounces_before_death_(bounces_before_death),
        exists_(true),
        is_stable_(true),
        radius_(std::max(area.get_width(), area.get_height()))
    {}

    PhysicsObject(AbstractNode *parent, const Rect<int> &area, const Vector2d<float> &velocity,
                  const Vector2d<float> &acceleration, float friction, int bounces_before_death,
                  const std::string &texture_file_name, const Rect<int> &texture_area = Rect<int>())
      : AbstractNode(parent, area, texture_file_name, texture_area),
        type_(PhysicsEntity::UNKNOWN),
        velocity_(velocity),
        acceleration_(acceleration),
        friction_(friction),
        bounces_before_death_(bounces_before_death),
        exists_(true),
        is_stable_(true),
        radius_(std::max(area.get_width(), area.get_height()))
    {}
    
    PhysicsEntity get_physics_entity_type() const
    {
        return type_;
    }

    bool handle_event(const Event &event) override
    {
        bool result = false;

        switch (event.get_type())
        {
            case EventType::TIME_PASSED:
            {
                acceleration_.set_y(6000);

                velocity_ += acceleration_ * Game::time_delta.count();
                // printf("velocity: %g %g\n", velocity_.x(), velocity_.y());

                float new_x = area_.left_top().x() + velocity_.x() * Game::time_delta.count();
                float new_y = area_.left_top().y() + velocity_.y() * Game::time_delta.count();

                acceleration_.set_x(0);
                acceleration_.set_y(0);
                is_stable_ = false;

                float angle = atan2f(velocity_.x(), velocity_.y());

                bool collision_happened = false;
                float response_x = 0;
                float response_y = 0;
                for (float r = angle - 3.14159f / 2.0f; r < angle + 3.14159f / 2.0f; r += 3.14159f / 8.0f)
                {
                    float test_pos_x = radius_ * cosf(r) + new_x;
                    float test_pos_y = radius_ * sinf(r) + new_y;

                    if (test_pos_x >= Game::map_width_)
                    {
                        test_pos_x = Game::map_width_ - 1;
                    }
                    if (test_pos_y >= Game::map_height_)
                    {
                        test_pos_y = Game::map_height_ - 1;
                    }
                    if (test_pos_x < 0)
                    {
                        test_pos_x = 0;
                    }
                    if (test_pos_y < 0)
                    {
                        test_pos_y = 0;
                    }
                    
                    if (Game::game->check_collision(this, get_physics_entity_type(),
                                                     {static_cast<int> (test_pos_x),
                                                     static_cast<int> (test_pos_y)}))
                    {
                        response_x += new_x - test_pos_x;
                        response_y += new_y - test_pos_y;

                        collision_happened = true;
                    }
                }

                float velocity_magnitude = sqrtf(velocity_.x() * velocity_.x() +
                                                    velocity_.y() * velocity_.y());
                if (collision_happened)
                {
                    is_stable_ = true;
                    
                    float response_magnitude = sqrtf(response_x * response_x +
                                                        response_y * response_y);
                    
                    // reflection vector
                    float dot = velocity_.x() * (response_x / response_magnitude) +
                                velocity_.y() * (response_y / response_magnitude);
                    velocity_.set_x((-2.0f * dot * (response_x / response_magnitude) + velocity_.x()) * friction_);
                    velocity_.set_y((-2.0f * dot * (response_y / response_magnitude) + velocity_.y()) * friction_); 
                }
                else
                {
                    area_.set_left_top_x(new_x);
                    area_.set_left_top_y(new_y);
                }

                if (velocity_magnitude < 0.1f)
                {
                    is_stable_ = true;

                    velocity_.set_x(0);
                    velocity_.set_y(0);
                }

                for (uint32_t child_index = 0; child_index < children_.size(); ++child_index)
                {
                    if (children_[child_index]->handle_event(event))
                    {
                        result = true;
                    }
                }

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
            }
        }

        return result;
    }

protected:

    PhysicsEntity type_;

    Vector2d<float> velocity_;
    Vector2d<float> acceleration_;
    float friction_;

    int bounces_before_death_;
    bool exists_; 

    bool is_stable_;

    float radius_;
};

#endif