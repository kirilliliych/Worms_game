#include "character.hpp"
#include "character_ui.hpp"
#include "weapon.hpp"
#include <math.h>


    Character::Character(AbstractNode *parent, const Rect<int> &area)
      : PhysicsObject(parent, area, {0, 0}, {0, 0}, DEFAULT_FRICTION, -1),
        crosshair_(new Crosshair(this, {10, 10, area_.center()},
                                 std::max(area.get_width(), area_.get_height()) + 30)),
        weapon_(new Weapon(this, area_, nullptr)),
        hp_(100),
        character_ui_(new CharacterUI(this, {area.get_width(), 5, {area.get_left_x(), area.get_top_y() - 10}}, hp_))
    {
        PhysicsObject::type_ = PhysicsEntity::CHARACTER;
    }

    Character::Character(AbstractNode *parent, const Rect<int> &area, const std::string &texture_file_name,
              const Rect<int> &texture_area)
      : PhysicsObject(parent, area, {0, 0}, {0, 0},
                      DEFAULT_FRICTION, -1, texture_file_name, texture_area),
        crosshair_(new Crosshair(this, {10, 10, area_.center()},
                                 std::max(area.get_width(), area_.get_height()) + 30)),
        weapon_(new Weapon(this, area_, nullptr)),
        hp_(100),
        character_ui_(new CharacterUI(this, {area.get_width(), 5, {area.get_left_x(), area.get_top_y() - 10}}, hp_))
    {
        PhysicsObject::type_ = PhysicsEntity::CHARACTER;

        calculate_scale();
    }

    Character::~Character()
    {
        delete crosshair_;
        delete weapon_;

        delete character_ui_;
    }

    int Character::get_hp() const
    {
        return hp_;
    }

    void Character::on_bounce_death(const Point2d<int> &death_position)
    {}

    void Character::render_self(Surface *surface, const Point2d<int> &camera_offset)
    {
        assert(surface != nullptr);

        Sprite self_sprite(*texture_, area_.left_top() - camera_offset + texture_->get_center());
        self_sprite.set_scale(crosshair_->is_right_semicircle() ? -texture_scale_ : texture_scale_, texture_scale_);
        self_sprite.set_origin(texture_->get_center().x(), texture_->get_center().y());
        surface->draw_sprite(self_sprite);
    }

    bool Character::handle_event(const Event &event)
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
                            // printf("set is_stable to false\n");
                            is_stable_ = false;

                            Game::game->lock_camera();

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

                if (children_handle_event(event))
                {
                    result = true;
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

                if (children_handle_event(event))
                {
                    result = true;
                }

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

                if (distance < event.eedata_.radius)
                {
                    float new_x_abs_velocity = (500.f * (1 - distance / event.eedata_.radius));
                    float new_y_abs_velocity = (500.f * (1 - distance / event.eedata_.radius));
                    velocity_.set_x(new_x_abs_velocity * dx_sign);
                    velocity_.set_y(-new_y_abs_velocity);

                    is_stable_ = false;
                }

                if (PhysicsObject::handle_event(event))
                {
                    result = true;
                }

                if (children_handle_event(event))
                {
                    result = true;
                }

                break;
            }

            case EventType::TIME_PASSED:
            {
                if (PhysicsObject::handle_event(event))
                {
                    result = true;
                }

                // weapon_->set_projectile_spawn_position(crosshair_->get_area().left_top());
                float crosshair_angle = crosshair_->get_angle();
                int radius = sqrtf(area_.half_size().x() * area_.half_size().x() +
                                   area_.half_size().y() * area_.half_size().y());
                weapon_->set_projectile_spawn_position(area_.center() + Point2d<int>(radius * cosf(crosshair_angle),
                                                                                     radius * sinf(crosshair_angle)));
                weapon_->set_OX_angle(crosshair_->get_angle());

                set_texture_by_angle_(crosshair_->get_angle());

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

    void Character::set_texture_by_angle_(float OX_angle)
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
        // printf("OX_angle_right_semicircle: %g\n", OX_angle_right_semicircle);

        float step = 3.14159f / 8;
        float border_angle = 3.14159f / 2 - step;
        for (uint32_t i = 0; i < 4; ++i)
        {
            if (OX_angle_right_semicircle >= border_angle)
            {
                bool loading_result = load_texture_from_image_manager(weapon_->get_weapon_traits()->get_image_file_name(i));
                assert(loading_result);

                return;
            }

            border_angle -= 2 * step;
        }

        load_texture_from_image_manager(weapon_->get_weapon_traits()->get_image_file_name(4));
        calculate_scale();
    }
