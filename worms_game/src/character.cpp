#include "character.hpp"
#include "character_ui.hpp"
#include "game.hpp"
#include "maths.hpp"
#include "weapon.hpp"


    Character::Character(AbstractNode *parent, const Rect<int> &area, uint32_t color)
      : PhysicsObject(parent, area, {0, 0}, {0, 0}, DEFAULT_FRICTION, -1),
        crosshair_(new Crosshair(this, {10, 10, area_.center()},
                                 std::max(area.width(), area_.height()) + 30, color)),
        weapon_(new Weapon(this, area_, nullptr)),
        hp_(100),
        character_ui_(new CharacterUI(this, {area.width(), 5, {area.left_x(), area.top_y() - 10}}, hp_, color))
    {
        PhysicsObject::type_ = PhysicsEntity::CHARACTER;
    }

    Character::Character(AbstractNode *parent, const Rect<int> &area, const std::string &texture_file_name, uint32_t color)
      : PhysicsObject(parent, area, {0, 0}, {0, 0},
                      DEFAULT_FRICTION, -1, texture_file_name),
        crosshair_(new Crosshair(this, {10, 10, area_.center()},
                                 std::max(area.width(), area_.height()) + 30, color)),
        weapon_(new Weapon(this, area_, nullptr)),
        hp_(100),
        character_ui_(new CharacterUI(this, {area.width(), 5, {area.left_x(), area.top_y() - 10}}, hp_, color))
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

    void Character::set_hp(int new_hp)
    {
        hp_ = new_hp;
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
                if (Game::game->does_player_have_control())
                {
                    if (Game::game->is_under_control(this) && is_stable_)
                    {
                        switch (event.kedata_.key_code)
                        {
                            case KeyboardKey::Enter:
                            {
                                // printf("CHARACTER CAUGHT KEY ENTER\n");
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
                    float throwing_scale = 1 - distance / event.eedata_.radius;
                    float new_x_abs_velocity = (600.f * (1 - distance / event.eedata_.radius));
                    float new_y_abs_velocity = (600.f * (1 - distance / event.eedata_.radius));
                    velocity_.set_x(new_x_abs_velocity * dx_sign);
                    velocity_.set_y(-new_y_abs_velocity);

                    assert(event.eedata_.radius - event.eedata_.full_damage_radius > 0);
                    float damage_scale = 1 - distance / (event.eedata_.radius - event.eedata_.full_damage_radius);
                    printf("damage_scale: %d\n", damage_scale);
                    assert(damage_scale >= 0);
                    int new_hp = get_hp() - event.eedata_.damage * damage_scale;
                    printf("new_hp: %d\n", new_hp);
                    set_hp(new_hp);

                    is_stable_ = false;
                }

                if (children_handle_event(event))
                {
                    result = true;
                }

                break;
            }

            case EventType::TIME_PASSED:
            {
                handle_physics();

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

    void Character::set_texture_by_angle_(float OX_angle)
    {
        if (weapon_->get_weapon_traits() == nullptr)
        {
            load_texture_from_image_manager("standing.png");
            calculate_scale();

            return;
        }

        float OX_angle_right_semicircle = OX_angle;
        if (OX_angle < -math_consts::HALF_PI)
        {
            OX_angle_right_semicircle = -math_consts::PI - OX_angle;
        }
        else if (OX_angle > math_consts::HALF_PI)
        {
            OX_angle_right_semicircle = math_consts::PI - OX_angle;
        }
        // printf("OX_angle_right_semicircle: %g\n", OX_angle_right_semicircle);

        float step = math_consts::PI / 8;
        float border_angle = math_consts::HALF_PI - step;
        
        uint32_t weapon_images_quantity = weapon_->get_weapon_traits()->get_images_quantity();
        for (uint32_t i = 0; i < weapon_images_quantity; ++i)
        {
            if (OX_angle_right_semicircle >= border_angle)
            {
                bool loading_result = load_texture_from_image_manager(weapon_->get_weapon_traits()->get_image_file_name(i));
                assert(loading_result);

                return;
            }

            border_angle -= 2 * step;
        }

        load_texture_from_image_manager(weapon_->get_weapon_traits()->get_image_file_name(weapon_images_quantity - 1));
        calculate_scale();
    }
