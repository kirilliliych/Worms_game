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
        radius_(std::max(area.get_width(), area.get_height()))
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
        radius_(sqrtf(area_.get_width()  * area.get_width() +
                      area_.get_height() * area_.get_height()))
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

        velocity_ += acceleration_ * Game::time_delta.count();

        float new_x = area_.left_top().x() + velocity_.x() * Game::time_delta.count();
        float new_y = area_.left_top().y() + velocity_.y() * Game::time_delta.count();
        float new_x_center = new_x + area_.get_width()  / 2.f;
        float new_y_center = new_y + area_.get_height() / 2.f;

        acceleration_.set_x(0);
        acceleration_.set_y(0);
        is_stable_ = false;

        if (std::abs(velocity_.y()) < 0.01)
        {
            velocity_.set_y(velocity_.y() > 0 ? 0.01 : -0.01);
        }
        float semicircle_flat_part_k_coef = -velocity_.x() / velocity_.y(); // k1 * k2 = -1
        bool above_semicircle_flat_part = true; //
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
        if (( above_semicircle_flat_part && (-area_.get_height() <= semicircle_flat_part_k_coef * -area_.get_width())) ||
            (!above_semicircle_flat_part && (-area_.get_height() >  semicircle_flat_part_k_coef * -area_.get_width())))
        {
            is_corner_in_correct_halfplane[0] = true;
        }
        if (( above_semicircle_flat_part && (-area_.get_height() <= semicircle_flat_part_k_coef * area_.get_width()))  ||
            (!above_semicircle_flat_part && (-area_.get_height() >  semicircle_flat_part_k_coef * area_.get_width())))
        {
            is_corner_in_correct_halfplane[1] = true;
        }
        if (( above_semicircle_flat_part && (area_.get_height() <= semicircle_flat_part_k_coef * area_.get_width()))   ||
            (!above_semicircle_flat_part && (area_.get_height() >  semicircle_flat_part_k_coef * area_.get_width())))
        {
            is_corner_in_correct_halfplane[2] = true;
        }
        if (( above_semicircle_flat_part && (area_.get_height() <= semicircle_flat_part_k_coef * -area_.get_width()))  ||
            (!above_semicircle_flat_part && (area_.get_height() >  semicircle_flat_part_k_coef * -area_.get_width())))
        {
            is_corner_in_correct_halfplane[3] = true;
        }


        bool collision_happened = false;
        float response_x = 0;
        float response_y = 0;

        if (is_corner_in_correct_halfplane[0])
        {
            for (float i = 0; i < area_.get_height(); i += 0.95f)
            {
                float test_pos_x = new_x;
                float test_pos_y = new_y + i;
                if (( above_semicircle_flat_part && (test_pos_y - new_y_center <= semicircle_flat_part_k_coef * (test_pos_x - new_x_center))) ||
                    (!above_semicircle_flat_part && (test_pos_y - new_y_center >  semicircle_flat_part_k_coef * (test_pos_x - new_x_center))))
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
                        response_x += new_x_center - test_pos_x;
                        response_y += new_y_center - test_pos_y;

                        collision_happened = true;
                    }
                }
            }

            for (float i = 0; i < area_.get_width(); i += 0.95f)
            {
                float test_pos_x = new_x + i;
                float test_pos_y = new_y;
                if (( above_semicircle_flat_part && (test_pos_y - new_y_center <= semicircle_flat_part_k_coef * (test_pos_x - new_x_center))) ||
                    (!above_semicircle_flat_part && (test_pos_y - new_y_center >  semicircle_flat_part_k_coef * (test_pos_x - new_x_center))))
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
                        response_x += new_x_center - test_pos_x;
                        response_y += new_y_center - test_pos_y;

                        collision_happened = true;
                    }
                }
            }
        }

        if (is_corner_in_correct_halfplane[2])
        {
            for (float i = 0; i < area_.get_height(); i += 0.95f)
            {
                float test_pos_x = new_x + area_.get_width() - 1;
                float test_pos_y = new_y + i;
                if (( above_semicircle_flat_part && (test_pos_y - new_y_center <= semicircle_flat_part_k_coef * (test_pos_x - new_x_center))) ||
                    (!above_semicircle_flat_part && (test_pos_y - new_y_center >  semicircle_flat_part_k_coef * (test_pos_x - new_x_center))))
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
                        response_x += new_x_center - test_pos_x;
                        response_y += new_y_center - test_pos_y;

                        collision_happened = true;
                    }
                }
            }

            for (float i = 0; i < area_.get_width(); i += 0.95f)
            {
                float test_pos_x = new_x + i;
                float test_pos_y = new_y + area_.get_height() - 1;
                if (( above_semicircle_flat_part && (test_pos_y - new_y_center <= semicircle_flat_part_k_coef * (test_pos_x - new_x_center))) ||
                    (!above_semicircle_flat_part && (test_pos_y - new_y_center >  semicircle_flat_part_k_coef * (test_pos_x - new_x_center))))
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
                        response_x += new_x_center - test_pos_x;
                        response_y += new_y_center - test_pos_y;

                        collision_happened = true;
                    }
                }
            }
        }

        if (is_corner_in_correct_halfplane[1])
        {
            if (!is_corner_in_correct_halfplane[2])
            {
                for (float i = 0; i < area_.get_height(); i += 0.95f)
                {
                    float test_pos_x = new_x + area_.get_width() - 1;
                    float test_pos_y = new_y + i;
                    if (( above_semicircle_flat_part && (test_pos_y - new_y_center <= semicircle_flat_part_k_coef * (test_pos_x - new_x_center))) ||
                        (!above_semicircle_flat_part && (test_pos_y - new_y_center >  semicircle_flat_part_k_coef * (test_pos_x - new_x_center))))
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
                            response_x += new_x_center - test_pos_x;
                            response_y += new_y_center - test_pos_y;

                            collision_happened = true;
                        }
                    }
                }
            }

            if (!is_corner_in_correct_halfplane[1])
            {
                for (float i = 0; i < area_.get_width(); i += 0.95f)
                {
                    float test_pos_x = new_x + i;
                    float test_pos_y = new_y;
                    if (( above_semicircle_flat_part && (test_pos_y - new_y_center <= semicircle_flat_part_k_coef * (test_pos_x - new_x_center))) ||
                        (!above_semicircle_flat_part && (test_pos_y - new_y_center >  semicircle_flat_part_k_coef * (test_pos_x - new_x_center))))
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
                            response_x += new_x_center - test_pos_x;
                            response_y += new_y_center - test_pos_y;
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
                for (float i = 0; i < area_.get_height(); i += 0.95f)
                {
                    float test_pos_x = new_x;
                    float test_pos_y = new_y + i;
                    if (( above_semicircle_flat_part && (test_pos_y - new_y_center <= semicircle_flat_part_k_coef * (test_pos_x - new_x_center))) ||
                        (!above_semicircle_flat_part && (test_pos_y - new_y_center >  semicircle_flat_part_k_coef * (test_pos_x - new_x_center))))
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
                            response_x += new_x_center - test_pos_x;
                            response_y += new_y_center - test_pos_y;

                            collision_happened = true;
                        }
                    }
                }
            }

            if (!is_corner_in_correct_halfplane[2])
            {
                for (float i = 0; i < area_.get_width(); i += 0.95f)
                {
                    float test_pos_x = new_x + i;
                    float test_pos_y = new_y + area_.get_height() - 1;
                    if (( above_semicircle_flat_part && (test_pos_y - new_y_center <= semicircle_flat_part_k_coef * (test_pos_x - new_x_center))) ||
                        (!above_semicircle_flat_part && (test_pos_y - new_y_center >  semicircle_flat_part_k_coef * (test_pos_x - new_x_center))))
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
                            response_x += new_x_center - test_pos_x;
                            response_y += new_y_center - test_pos_y;

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
                        // Game::game->set_camera_tracking_object(Game::game->get_character_under_control());
                    }
                    // printf("character set as center of camera\n");
                    on_bounce_death(area_.center());
                }
            }
        }
        else
        {
            area_.set_left_top_x(new_x);
            area_.set_left_top_y(new_y);
        }

        if (velocity_magnitude < 1.f)
        {
            // printf("set to true\n");
            is_stable_ = true;

            // velocity_.set_x(0);
            // velocity_.set_y(0);
        }
    }

    bool PhysicsObject::handle_event(const Event &event)
    {
        bool result = false;
        // printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");

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
