#include "character.hpp"
#include "game.hpp"
#include "physics_entity.hpp"
#include "physics_object.hpp"


PhysicsObject::PhysicsObject(AbstractNode *parent, const Rect<int> &area, const Vector2d<float> &velocity,
                             const Vector2d<float> &acceleration, float friction, int bounces_before_death)
  : AbstractNode(parent, area),
    type_(PhysicsEntity::UNKNOWN),
    velocity_(velocity),
    acceleration_(acceleration),
    friction_(friction),
    bounces_before_death_(bounces_before_death),
    is_stable_(true),
    radius_(std::max(area.width(), area.height()))
{}

PhysicsObject::PhysicsObject(AbstractNode *parent, const Rect<int> &area, const Vector2d<float> &velocity,
                             const Vector2d<float> &acceleration, float friction, int bounces_before_death,
                             const std::string &texture_file_name)
  : AbstractNode(parent, area, texture_file_name),
    type_(PhysicsEntity::UNKNOWN),
    velocity_(velocity),
    acceleration_(acceleration),
    friction_(friction),
    bounces_before_death_(bounces_before_death),
    is_stable_(true),
    radius_(sqrtf(area_.width()  * area.width() +
                  area_.height() * area_.height()))
{}

PhysicsEntity PhysicsObject::get_physics_entity_type() const
{
    return type_;
}

bool PhysicsObject::is_stable() const
{
    return is_stable_;
}

void PhysicsObject::handle_physics()
{
    acceleration_.set_y(2000);
    // assert(acceleration_ == Vector2d<float> (0, 2000));

    velocity_ += acceleration_ * Game::game->time_delta.count();

    float new_x = area_.left_top().x() + velocity_.x() * Game::game->time_delta.count();
    float new_y = area_.left_top().y() + velocity_.y() * Game::game->time_delta.count();

    Rect<int> new_position(get_area().width(), get_area().height(),
                             {static_cast<int> (new_x),
                                        static_cast<int> (new_y)
                                       });
    int new_position_center_x = new_position.center().x();
    int new_position_center_y = new_position.center().y();

    acceleration_.set_x(0);
    acceleration_.set_y(0);
    is_stable_ = false;

    float velocity_threshold = 0.01f;
    if (std::abs(velocity_.y()) < velocity_threshold)
    {
        velocity_.set_y(velocity_.y() > 0 ? velocity_threshold : -velocity_threshold);
    }
    float semicircle_flat_part_k_coef = -velocity_.x() / velocity_.y(); // k1 * k2 = -1
    bool above_semicircle_flat_part = true;
    float velocity_magnitude = sqrtf(velocity_.x() * velocity_.x() +
                                       velocity_.y() * velocity_.y());
    Vector2d<float> velocity_normalized{velocity_.x() / velocity_magnitude,
                                        velocity_.y() / velocity_magnitude
                                       };
    if (velocity_normalized.y() > semicircle_flat_part_k_coef * velocity_normalized.x())
    {
        above_semicircle_flat_part = false;
    }

    std::vector<bool> is_corner_in_correct_halfplane(4, false);

    if (point_is_in_correct_semicircle_zone_(semicircle_flat_part_k_coef,
                                             above_semicircle_flat_part,
                                             Point2d<int>(-area_.width(),
                                                                -area_.height())))
    {
        is_corner_in_correct_halfplane[0] = true;
    }

    if (point_is_in_correct_semicircle_zone_(semicircle_flat_part_k_coef,
                                             above_semicircle_flat_part,
                                             Point2d<int>( area_.width(),
                                                                -area_.height())))
    {
        is_corner_in_correct_halfplane[1] = true;
    }

    if (point_is_in_correct_semicircle_zone_(semicircle_flat_part_k_coef, 
                                             above_semicircle_flat_part,
                                             Point2d<int>( area_.width(),
                                                                 area_.height())))
    {
        is_corner_in_correct_halfplane[2] = true;
    }

    if (point_is_in_correct_semicircle_zone_(semicircle_flat_part_k_coef,
                                             above_semicircle_flat_part,
                                             Point2d<int> (-area_.width(),
                                                                  area_.height())))
    {
        is_corner_in_correct_halfplane[3] = true;
    }

    // if (( above_semicircle_flat_part && (-area_.height() <= semicircle_flat_part_k_coef * -area_.width())) ||
    //     (!above_semicircle_flat_part && (-area_.height() >  semicircle_flat_part_k_coef * -area_.width())))
    // {
    //     is_corner_in_correct_halfplane[0] = true;
    // }
    // if (( above_semicircle_flat_part && (-area_.height() <= semicircle_flat_part_k_coef * area_.width()))  ||
    //     (!above_semicircle_flat_part && (-area_.height() >  semicircle_flat_part_k_coef * area_.width())))
    // {
    //     is_corner_in_correct_halfplane[1] = true;
    // }
    // if (( above_semicircle_flat_part && (area_.height() <= semicircle_flat_part_k_coef * area_.width()))   ||
    //     (!above_semicircle_flat_part && (area_.height() >  semicircle_flat_part_k_coef * area_.width())))
    // {
    //     is_corner_in_correct_halfplane[2] = true;
    // }
    // if (( above_semicircle_flat_part && (area_.height() <= semicircle_flat_part_k_coef * -area_.width()))  ||
    //     (!above_semicircle_flat_part && (area_.height() >  semicircle_flat_part_k_coef * -area_.width())))
    // {
    //     is_corner_in_correct_halfplane[3] = true;
    // }

    bool collision_happened = false;
    float response_x = 0;
    float response_y = 0;

    // void check_collision         // make function to wrap collision checks below

    if (is_corner_in_correct_halfplane[0])
    {
        for (float i = 0; i < area_.height(); i += 0.95f)
        {
            float test_pos_x = new_x;
            float test_pos_y = new_y + i;
            if (( above_semicircle_flat_part && (test_pos_y - new_position_center_y <= semicircle_flat_part_k_coef * (test_pos_x - new_position_center_x))) ||
                (!above_semicircle_flat_part && (test_pos_y - new_position_center_y >  semicircle_flat_part_k_coef * (test_pos_x - new_position_center_x))))
            {
                Event check_collision_event;
                check_collision_event.set_type(EventType::COLLISION_EVENT);
                check_collision_event.cedata_.checker = get_physics_entity_type();
                check_collision_event.cedata_.checker_address = this;
                check_collision_event.cedata_.position = {static_cast<int> (test_pos_x),
                                                          static_cast<int> (test_pos_y)
                                                         }; 
                if (Game::game->launch_event(check_collision_event))
                {
                    response_x += new_position_center_x - test_pos_x;
                    response_y += new_position_center_y - test_pos_y;

                    collision_happened = true;
                }
            }
        }

        for (float i = 0; i < area_.width(); i += 0.95f)
        {
            float test_pos_x = new_x + i;
            float test_pos_y = new_y;
            if (( above_semicircle_flat_part && (test_pos_y - new_position_center_y <= semicircle_flat_part_k_coef * (test_pos_x - new_position_center_x))) ||
                (!above_semicircle_flat_part && (test_pos_y - new_position_center_y >  semicircle_flat_part_k_coef * (test_pos_x - new_position_center_x))))
            {
                Event check_collision_event;
                check_collision_event.set_type(EventType::COLLISION_EVENT);
                check_collision_event.cedata_.checker = get_physics_entity_type();
                check_collision_event.cedata_.checker_address = this;
                check_collision_event.cedata_.position = {static_cast<int> (test_pos_x),
                                                          static_cast<int> (test_pos_y)
                                                         }; 
                if (Game::game->launch_event(check_collision_event))
                {
                    response_x += new_position_center_x - test_pos_x;
                    response_y += new_position_center_y - test_pos_y;

                    collision_happened = true;
                }
            }
        }
    }

    if (is_corner_in_correct_halfplane[2])
    {
        for (float i = 0; i < area_.height(); i += 0.95f)
        {
            float test_pos_x = new_x + area_.width() - 1;
            float test_pos_y = new_y + i;
            if (( above_semicircle_flat_part && (test_pos_y - new_position_center_y <= semicircle_flat_part_k_coef * (test_pos_x - new_position_center_x))) ||
                (!above_semicircle_flat_part && (test_pos_y - new_position_center_y >  semicircle_flat_part_k_coef * (test_pos_x - new_position_center_x))))
            {
                Event check_collision_event;
                check_collision_event.set_type(EventType::COLLISION_EVENT);
                check_collision_event.cedata_.checker = get_physics_entity_type();
                check_collision_event.cedata_.checker_address = this;
                check_collision_event.cedata_.position = {static_cast<int> (test_pos_x),
                                                          static_cast<int> (test_pos_y)
                                                         }; 
                if (Game::game->launch_event(check_collision_event))
                {
                    response_x += new_position_center_x - test_pos_x;
                    response_y += new_position_center_y - test_pos_y;

                    collision_happened = true;
                }
            }
        }

        for (float i = 0; i < area_.width(); i += 0.95f)
        {
            float test_pos_x = new_x + i;
            float test_pos_y = new_y + area_.height() - 1;
            if (( above_semicircle_flat_part && (test_pos_y - new_position_center_y <= semicircle_flat_part_k_coef * (test_pos_x - new_position_center_x))) ||
                (!above_semicircle_flat_part && (test_pos_y - new_position_center_y >  semicircle_flat_part_k_coef * (test_pos_x - new_position_center_x))))
            {
                Event check_collision_event;
                check_collision_event.set_type(EventType::COLLISION_EVENT);
                check_collision_event.cedata_.checker = get_physics_entity_type();
                check_collision_event.cedata_.checker_address = this;
                check_collision_event.cedata_.position = {static_cast<int> (test_pos_x),
                                                          static_cast<int> (test_pos_y)
                                                         }; 
                if (Game::game->launch_event(check_collision_event))
                {
                    response_x += new_position_center_x - test_pos_x;
                    response_y += new_position_center_y - test_pos_y;

                    collision_happened = true;
                }
            }
        }
    }

    if (is_corner_in_correct_halfplane[1])
    {
        if (!is_corner_in_correct_halfplane[2])
        {
            for (float i = 0; i < area_.height(); i += 0.95f)
            {
                float test_pos_x = new_x + area_.width() - 1;
                float test_pos_y = new_y + i;
                if (( above_semicircle_flat_part && (test_pos_y - new_position_center_y <= semicircle_flat_part_k_coef * (test_pos_x - new_position_center_x))) ||
                    (!above_semicircle_flat_part && (test_pos_y - new_position_center_y >  semicircle_flat_part_k_coef * (test_pos_x - new_position_center_x))))
                {
                    Event check_collision_event;
                    check_collision_event.set_type(EventType::COLLISION_EVENT);
                    check_collision_event.cedata_.checker = get_physics_entity_type();
                    check_collision_event.cedata_.checker_address = this;
                    check_collision_event.cedata_.position = {static_cast<int> (test_pos_x),
                                                              static_cast<int> (test_pos_y)
                                                            }; 
                    if (Game::game->launch_event(check_collision_event))
                    {
                        response_x += new_position_center_x - test_pos_x;
                        response_y += new_position_center_y - test_pos_y;

                        collision_happened = true;
                    }
                }
            }
        }

        if (!is_corner_in_correct_halfplane[1])
        {
            for (float i = 0; i < area_.width(); i += 0.95f)
            {
                float test_pos_x = new_x + i;
                float test_pos_y = new_y;
                if (( above_semicircle_flat_part && (test_pos_y - new_position_center_y <= semicircle_flat_part_k_coef * (test_pos_x - new_position_center_x))) ||
                    (!above_semicircle_flat_part && (test_pos_y - new_position_center_y >  semicircle_flat_part_k_coef * (test_pos_x - new_position_center_x))))
                {
                    Event check_collision_event;
                    check_collision_event.set_type(EventType::COLLISION_EVENT);
                    check_collision_event.cedata_.checker = get_physics_entity_type();
                    check_collision_event.cedata_.checker_address = this;
                    check_collision_event.cedata_.position = {static_cast<int> (test_pos_x),
                                                              static_cast<int> (test_pos_y)
                                                            }; 
                    if (Game::game->launch_event(check_collision_event))
                    {
                        response_x += new_position_center_x - test_pos_x;
                        response_y += new_position_center_y - test_pos_y;
                        collision_happened = true;
                    }
                }
            }
        }
    }

    if (is_corner_in_correct_halfplane[3])
    {
        if (!is_corner_in_correct_halfplane[0])
        {
            for (float i = 0; i < area_.height(); i += 0.95f)
            {
                float test_pos_x = new_x;
                float test_pos_y = new_y + i;
                if (( above_semicircle_flat_part && (test_pos_y - new_position_center_y <= semicircle_flat_part_k_coef * (test_pos_x - new_position_center_x))) ||
                    (!above_semicircle_flat_part && (test_pos_y - new_position_center_y >  semicircle_flat_part_k_coef * (test_pos_x - new_position_center_x))))
                {
                    Event check_collision_event;
                    check_collision_event.set_type(EventType::COLLISION_EVENT);
                    check_collision_event.cedata_.checker = get_physics_entity_type();
                    check_collision_event.cedata_.checker_address = this;
                    check_collision_event.cedata_.position = {static_cast<int> (test_pos_x),
                                                              static_cast<int> (test_pos_y)
                                                             }; 
                    if (Game::game->launch_event(check_collision_event))
                    {
                        response_x += new_position_center_x - test_pos_x;
                        response_y += new_position_center_y - test_pos_y;

                        collision_happened = true;
                    }
                }
            }
        }

        if (!is_corner_in_correct_halfplane[2])
        {
            for (float i = 0; i < area_.width(); i += 0.95f)
            {
                float test_pos_x = new_x + i;
                float test_pos_y = new_y + area_.height() - 1;
                if (( above_semicircle_flat_part && (test_pos_y - new_position_center_y <= semicircle_flat_part_k_coef * (test_pos_x - new_position_center_x))) ||
                    (!above_semicircle_flat_part && (test_pos_y - new_position_center_y >  semicircle_flat_part_k_coef * (test_pos_x - new_position_center_x))))
                {
                    Event check_collision_event;
                    check_collision_event.set_type(EventType::COLLISION_EVENT);
                    check_collision_event.cedata_.checker = get_physics_entity_type();
                    check_collision_event.cedata_.checker_address = this;
                    check_collision_event.cedata_.position = {static_cast<int> (test_pos_x),
                                                              static_cast<int> (test_pos_y)
                                                             }; 
                    if (Game::game->launch_event(check_collision_event))
                    {
                        response_x += new_position_center_x - test_pos_x;
                        response_y += new_position_center_y - test_pos_y;

                        collision_happened = true;
                    }
                }
            }
        }
    }

    if (collision_happened && !is_stable_)
    {
        is_stable_ = true;
                
        float response_magnitude = sqrtf(response_x * response_x +
                                            response_y * response_y);
                
        // reflection vector
        float response_x_norm = response_x / response_magnitude;
        float response_y_norm = response_y / response_magnitude;
        // printf("response_x_norm: %g\n", response_x_norm);
        // printf("response_y_norm: %g\n", response_y_norm);
        float dot = velocity_.x() * response_x_norm +
                    velocity_.y() * response_y_norm;

        // printf("before collision velocity is %g %g\n", velocity_.x(), velocity_.y());
        velocity_.set_x((-2.0f * dot * response_x_norm + velocity_.x()) * friction_);
        velocity_.set_y((-2.0f * dot * response_y_norm + velocity_.y()) * friction_);
        // printf("after collision velocity is %g %g\n", velocity_.x(), velocity_.y());

        if (bounces_before_death_ > 0)
        {
            --bounces_before_death_;
            exists_ = bounces_before_death_ > 0;

            if (!exists_)
            {
                if (this == Game::game->get_camera_tracking_object())
                {
                    Game::game->set_camera_tracking_object(nullptr);
                }
                on_bounce_death(area_.center());
            }
        }
    }
    else
    {
        area_.set_left_top_x(new_x);
        area_.set_left_top_y(new_y);
    }


    if ((get_physics_entity_type() == PhysicsEntity::CHARACTER) && (velocity_magnitude < 300.f))
    {
        is_stable_ = true;
    }
    else if (velocity_magnitude < 100.f)
    {
        is_stable_ = true;
    }
}

bool PhysicsObject::handle_event(const Event &event)
{
    bool result = false;

    switch (event.get_type())
    {
        case EventType::TIME_PASSED:
        {
            handle_physics();

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
