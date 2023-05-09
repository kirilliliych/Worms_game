#pragma once


#include "abstract_node.hpp"
#include "crosshair.hpp"
#include "debris.hpp"
#include "physics_entity.hpp"
#include "physics_object.hpp"
#include "sfmlwrap/events/event.hpp"
#include "sfmlwrap/texture.hpp"
#include "weapon.hpp"


class Character : public PhysicsObject
{
public:

    Character(AbstractNode *parent, const Rect<int> &area)
      : PhysicsObject(parent, area, {0, 0}, {0, 0}, DEFAULT_FRICTION, -1),
        crosshair_(new Crosshair(this, {10, 10, area_.center()},
                                 std::max(area.get_width(), area_.get_height()) + 30)),
        weapon_(new Weapon(this, area_, nullptr))
    {
        PhysicsObject::type_ = PhysicsEntity::CHARACTER;
    }

    Character(AbstractNode *parent, const Rect<int> &area, const std::string &texture_file_name,
              const Rect<int> &texture_area = Rect<int>())
      : PhysicsObject(parent, area, {0, 0}, {0, 0},
                      DEFAULT_FRICTION, -1, texture_file_name, texture_area),
        crosshair_(new Crosshair(this, {10, 10, area_.center()},
                                 std::max(area.get_width(), area_.get_height()) + 30)),
        weapon_(new Weapon(this, area_, nullptr))
    {
        PhysicsObject::type_ = PhysicsEntity::CHARACTER;

        // uint32_t texture_width  = texture_->get_width();
        // uint32_t texture_height = texture_->get_height();
        // float asked_width  = static_cast<float> (area.get_width());
        // float asked_height = static_cast<float> (area.get_height());
        // float x_scale = asked_width  / static_cast<float> (texture_width);
        // float y_scale = asked_height / static_cast<float> (texture_height);
        // if (y_scale < x_scale)
        // {
        //     area_.set_width(y_scale * static_cast<int> (texture_width));
        //     texture_scale_ = y_scale;
        // }
        // else
        // {
        //     area_.set_height(x_scale * static_cast<int> (texture_height));
        //     texture_scale_ = x_scale;
        // }

        calculate_scale();
    }

    ~Character()
    {
        delete crosshair_;
        delete weapon_;
    }

    void on_bounce_death(const Point2d<int> &death_position) override
    {}

    void render_self(Surface *surface, const Point2d<int> &camera_offset) override
    {
        assert(surface != nullptr);

        Sprite self_sprite(*texture_, area_.left_top() - camera_offset);
        self_sprite.set_scale(crosshair_->is_right_semicircle() ? -texture_scale_ : texture_scale_, texture_scale_);
        self_sprite.set_origin(texture_->get_center().x(), 0);
        surface->draw_sprite(self_sprite);
    }

    bool handle_event(const Event &event) override
    {
        bool result = false;

        switch (event.get_type())
        {
            case EventType::KEY_PRESSED:
            {
                if (Game::game->is_under_control(this) && 
                    (is_stable_))
                {
                    switch (event.kedata_.key_code)
                    {
                        // case KeyboardKey::Z:
                        // {
                        //     printf("CHARACTER CAUGHT KEY Z\n");
                        //     velocity_.set_x(-200.0f * cosf(crosshair_->get_angle()));
                        //     velocity_.set_y(-400.0f * -sinf(crosshair_->get_angle()));
                        //     is_stable_ = false;

                        //     break;
                        // }
                        
                        case KeyboardKey::Enter:
                        {
                            // printf("CHARACTER CAUGHT KEY X\n");
                            velocity_.set_x(400.0f * cosf(crosshair_->get_angle()));
                            velocity_.set_y(400.0f * sinf(crosshair_->get_angle()));
                            // printf("new velocity is %g %g\n", velocity_.x(), velocity_.y());
                            is_stable_ = false;

                            break;
                        }

                        case KeyboardKey::Num0:
                        {
                            weapon_->set_weapon_traits(nullptr);

                            break;
                        }

                        case KeyboardKey::Num1:
                        {
                            printf("changing weapon to rocket launcher\n");
                            weapon_->set_weapon_traits(&traits::weapon_traits_pool[Weapons::ROCKET_LAUNCHER]);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }
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

            case EventType::COLLISION_EVENT:
            {
                if ((event.cedata_.checker == PhysicsEntity::PROJECTILE) && 
                    (area_.contains(event.cedata_.position)))
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

            case EventType::EXPLOSION_EVENT:
            {
                float center_x = area_.center_x();
                float center_y = area_.center_y();
                float dx = center_x - event.eedata_.position.x();
                float dy = center_y - event.eedata_.position.y();
                int dx_sign = dx >= 0 ? dx > 0 ? 1 : 0 : -1;
                float distance = sqrtf(dx * dx + dy * dy);
                // printf("dx: %g dy: %g distance: %g\n", dx, dy, distance);
                // if (distance < 1)        // first version
                // {
                //     // printf("distance too small\n");
                //     distance = 1;
                //     dx = 1;
                //     dy = 1;
                // }
                if (distance < event.eedata_.radius)
                {   
                    // printf("bomb reached\n");
                    // float new_x_velocity = 10.f * dx / distance * event.eedata_.radius;      // first version
                    // float new_y_velocity = 20.f * dy / distance * event.eedata_.radius;
                    // new_x_velocity = new_x_velocity < 0 ? std::max(new_x_velocity, -1000.f) : std::min(new_x_velocity, 1000.f);
                    // new_y_velocity = new_y_velocity < 0 ? std::max(new_y_velocity, -1000.f) : std::min(new_y_velocity, 1000.f);

                    // velocity_.set_x(new_x_velocity);
                    // velocity_.set_y(new_y_velocity);
                    // // for (int i = 0; i < 1000; ++i)

                    float new_x_abs_velocity = (1000.f * (1 - distance / event.eedata_.radius));    // second version
                    float new_y_abs_velocity = (1000.f * (1 - distance / event.eedata_.radius));
                    velocity_.set_x(new_x_abs_velocity * dx_sign);
                    velocity_.set_y(-new_y_abs_velocity);
                    // printf("VELOCITY AAAA: x %g y %g\n", velocity_.x(), velocity_.y());

                    is_stable_ = false;
                }

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
                // result = children_handle_event(event);      // double handling

                break;
            }

            case EventType::TIME_PASSED:
            {
                if (PhysicsObject::handle_event(event))
                {
                    result = true;
                }

                weapon_->set_projectile_spawn_position(crosshair_->get_area().left_top());
                weapon_->set_OX_angle(crosshair_->get_angle());

                set_texture_by_angle_(crosshair_->get_angle());;

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

    void set_texture_by_angle_(float OX_angle)
    {
        if (weapon_->get_weapon_traits() == nullptr)
        {
            load_texture_from_image_manager("standing.png");
            calculate_scale();

            return;
        }

        float OX_angle_right_semicircle = OX_angle;
        if (OX_angle < -3.14159f / 2)
        {
            OX_angle_right_semicircle = -3.14159f - OX_angle;
        }
        else if (OX_angle > 3.14159f / 2)
        {
            OX_angle_right_semicircle = 3.14159f - OX_angle;
        }
        printf("OX_angle_right_semicircle: %g\n", OX_angle_right_semicircle);

        // assert(0);
        float step = 3.14159f / 8;
        float border_angle = 3.14159f / 2 - step;
        for (uint32_t i = 0; i < 4; ++i)
        {
            if (OX_angle_right_semicircle >= border_angle)
            {
                // assert(0);
                assert(load_texture_from_image_manager(weapon_->get_weapon_traits()->get_image_file_name(i)));
                // assert(0);

                return;
            }

            border_angle -= 2 * step;
        }

        load_texture_from_image_manager(weapon_->get_weapon_traits()->get_image_file_name(4));
        calculate_scale();
    }

private:
public:

    static constexpr float DEFAULT_FRICTION = 0.0008;

    Crosshair *crosshair_;    
    Weapon *weapon_;

};
