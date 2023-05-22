#include "character.hpp"
#include "character_ui.hpp"
#include "game.hpp"
#include "maths.hpp"
#include "sfmlwrap/image.hpp"
#include "weapon.hpp"


Character::Character(AbstractNode *parent, const Rect<int> &area, int hp, uint32_t color)
  : PhysicsObject(parent, area, {0, 0}, {0, 0}, DEFAULT_FRICTION, -1),
    state_(CharacterState::PASSIVE),
    animation_cur_image_name_(""),
    animation_image_change_cur_delay_(std::rand() % static_cast<int> (ANIMATION_IMAGE_CHANGE_MIN_DELAY)),
    switch_to_passive_stance_(false),
    crosshair_(new Crosshair(this, {10, 10, area_.center()},
                             std::max(area.width(), area_.height()) + 30, color)),
    weapon_(new Weapon(this, area_, nullptr)),
    hp_(hp),
    character_ui_(new CharacterUI(this, {area.width(), 5, {area.left_x(), area.top_y() - 10}}, hp_, color))
{
    PhysicsObject::type_ = PhysicsEntity::CHARACTER;
}

Character::Character(AbstractNode *parent, const Rect<int> &area, int hp, const std::string &image_file_name, uint32_t color)
  : PhysicsObject(parent, area, {0, 0}, {0, 0},
                  DEFAULT_FRICTION, -1, image_file_name),
    state_(CharacterState::PASSIVE),
    animation_cur_image_name_(image_file_name),
    animation_image_change_cur_delay_(std::rand() % static_cast<int> (ANIMATION_IMAGE_CHANGE_MIN_DELAY)),
    switch_to_passive_stance_(false),
    crosshair_(new Crosshair(this, {10, 10, area_.center()},
                             std::max(area.width(), area_.height()) + 30, color)),
    weapon_(new Weapon(this, area_, nullptr)),
    hp_(hp),
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

bool Character::is_alive() const
{
    return hp_ > 0;
}

int Character::get_hp() const
{
    return hp_;
}

void Character::set_hp(int new_hp)
{
    hp_ = new_hp;
}

void Character::remove_weapon()
{
    state_ = CharacterState::PASSIVE;
}

void Character::on_bounce_death(const Point2d<int> &death_position)
{}

void Character::kill()
{
    set_hp(-1);
    state_ = CharacterState::DEAD;

    if (this == Game::game->get_character_under_control())
    {
        Game::game->finish_player_action();
        Game::game->set_player_control(false);
    }
}

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
    // if (state_ == CharacterState::DEAD)
    // {
    //     set_texture_(crosshair_->get_angle());

    //     return false;
    // }

    bool result = false;

    switch (event.get_type())
    {
        case EventType::KEY_PRESSED:
        {
            if (Game::game->does_player_have_control() && Game::game->is_under_control(this) &&
                ((state_ == CharacterState::ARMED) || (state_ == CharacterState::PASSIVE)))
            {
                    switch (event.kedata_.key_code)
                    {
                        case KeyboardKey::Enter:
                        {
                            float velocity_x_offset = 80.f;
                            velocity_.set_x(400.0f * cosf(crosshair_->get_angle()) + velocity_x_offset);
                            velocity_.set_y(400.0f * sinf(crosshair_->get_angle()));
                            is_stable_ = false;

                            state_ = CharacterState::MOVING;

                            Game::game->lock_camera();

                            break;
                        }

                        case KeyboardKey::Num0:
                        {
                            weapon_->set_weapon_traits(nullptr);

                            state_ = CharacterState::PASSIVE;

                            break;
                        }

                        case KeyboardKey::Num1:
                        {
                            printf("changing weapon to rocket launcher\n");
                            weapon_->set_weapon_traits(&traits::weapon_traits_pool[Weapons::ROCKET_LAUNCHER]);

                            state_ = CharacterState::ARMED;

                            break;
                        }

                        default:
                        {
                            break;
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
            if (state_ != CharacterState::DEAD)
            {
                float center_x = area_.center_x();
                float center_y = area_.center_y();
                float dx = center_x - event.eedata_.position.x();
                float dy = center_y - event.eedata_.position.y();
                int dx_sign = dx >= 0 ? dx > 0 ? 1 : 0 : -1;
                float distance = sqrtf(dx * dx + dy * dy);

                if (distance < event.eedata_.radius)
                {
                    assert(event.eedata_.radius - event.eedata_.full_damage_radius > 0);
                    float damage_scale = std::max(distance / (event.eedata_.radius - event.eedata_.full_damage_radius), 0.f);
                    assert(damage_scale >= 0);
                    int new_hp = get_hp() - event.eedata_.damage * damage_scale;
                    set_hp(new_hp);
                    if (new_hp <= 0)
                    {
                        state_ = CharacterState::DEAD;
                    }
                    else
                    {
                        float throwing_force_scale = 1 - distance / event.eedata_.radius;
                        float new_x_abs_velocity = (600.f * throwing_force_scale);
                        float new_y_abs_velocity = (600.f * throwing_force_scale);
                        velocity_.set_x(new_x_abs_velocity * dx_sign);
                        velocity_.set_y(-new_y_abs_velocity);

                        is_stable_ = false;

                        state_ = CharacterState::HIT;
                    }
                }

                if (children_handle_event(event))
                {
                    result = true;
                }
            }

            break;
        }

        case EventType::TIME_PASSED:
        {
            set_texture_(crosshair_->get_angle());
            handle_physics();

            if (state_ != CharacterState::DEAD)
            {
                float crosshair_angle = crosshair_->get_angle();
                int radius = sqrtf(area_.half_size().x() * area_.half_size().x() +
                                      area_.half_size().y() * area_.half_size().y());
                weapon_->set_projectile_spawn_position(area_.center() + Point2d<int>(radius * cosf(crosshair_angle),
                                                                                               radius * sinf(crosshair_angle)));
                weapon_->set_OX_angle(crosshair_->get_angle());

                if (is_stable_ && ((state_ == CharacterState::MOVING) || (state_ == CharacterState::HIT)))
                {
                    state_ = CharacterState::PASSIVE;
                }
                if (state_ != CharacterState::PASSIVE)
                {
                    animation_image_change_cur_delay_ = ANIMATION_IMAGE_CHANGE_MIN_DELAY;
                }
                else
                {
                    if (switch_to_passive_stance_)
                    {
                        switch_to_passive_stance_ = false;
                        animation_image_change_cur_delay_ = Game::game->time_delta.count();
                    }
                    else
                    {
                        animation_image_change_cur_delay_ += Game::game->time_delta.count();
                    }
                }

                if (children_handle_event(event))
                {
                    result = true;
                }
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

            break;
        }
    }

    return result;
}

void Character::set_texture_(float OX_angle)
{
    std::string new_image = "";

    switch (state_)
    {
        case CharacterState::PASSIVE:
        {
            int seed = std::rand() % 10;
            switch (seed)
            {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                {
                    new_image = "passive1.png";

                    break;
                }

                case 8:
                {
                    new_image = "passive2.png";

                    break;
                }

                case 9:
                {
                    new_image = "passive3.png";

                    break;
                }

                default:
                {
                    break;
                }
            }

            if ((new_image != animation_cur_image_name_) && (animation_image_change_cur_delay_ >= ANIMATION_IMAGE_CHANGE_MIN_DELAY))
            {
                animation_image_change_cur_delay_ = 0.f;
                load_texture_from_image_manager(new_image);
                animation_cur_image_name_ = new_image;
            }

            break;
        }

        case CharacterState::MOVING:
        {
            new_image = "moving.png";
            if (new_image != animation_cur_image_name_)
            {
                load_texture_from_image_manager(new_image);
                animation_cur_image_name_ = new_image;
            }

            break;
        }

        case CharacterState::ARMED:
        {
            float OX_angle_right_semicircle = OX_angle;
            if (OX_angle < -math_consts::HALF_PI)
            {
                OX_angle_right_semicircle = -math_consts::PI - OX_angle;
            }
            else if (OX_angle > math_consts::HALF_PI)
            {
                OX_angle_right_semicircle = math_consts::PI - OX_angle;
            }

            float step = math_consts::PI / 8;
            float border_angle = math_consts::HALF_PI - step;

            uint32_t weapon_images_quantity = weapon_->get_weapon_traits()->get_images_quantity();
            for (uint32_t i = 0; i < weapon_images_quantity; ++i)
            {
                if (OX_angle_right_semicircle >= border_angle)
                {
                    new_image = weapon_->get_weapon_traits()->get_image_file_name(i);
                    if (new_image != animation_cur_image_name_)
                    {
                        bool loading_result = load_texture_from_image_manager(new_image);
                        assert(loading_result);
                        animation_cur_image_name_ = new_image;
                    }

                    return;
                }

                border_angle -= 2 * step;
            }

            new_image = weapon_->get_weapon_traits()->get_image_file_name(weapon_images_quantity - 1);
            if (new_image != animation_cur_image_name_)
            {
                load_texture_from_image_manager(new_image);
                animation_cur_image_name_ = new_image;
            }

            break;
        }

        case CharacterState::HIT:
        {
            new_image = "hit.png";
            if (new_image != animation_cur_image_name_)
            {
                load_texture_from_image_manager(new_image);
                animation_cur_image_name_ = new_image;
            }

            break;
        }

        case CharacterState::DEAD:
        {
            new_image = "tombstone.png";
            if (new_image != animation_cur_image_name_)
            {
                load_texture_from_image_manager(new_image);
                animation_cur_image_name_ = new_image;
            }

            break;
        }
        
        default:
        {
            std::cerr << "ERROR: unknown character state" << std::endl;

            break;
        }
    }

    // if (weapon_->get_weapon_traits() == nullptr)
    // {
    //     load_texture_from_image_manager("tombstone.png");
    //     calculate_scale();

    //     return;
    // }

    // float OX_angle_right_semicircle = OX_angle;
    // if (OX_angle < -math_consts::HALF_PI)
    // {
    //     OX_angle_right_semicircle = -math_consts::PI - OX_angle;
    // }
    // else if (OX_angle > math_consts::HALF_PI)
    // {
    //     OX_angle_right_semicircle = math_consts::PI - OX_angle;
    // }

    // float step = math_consts::PI / 8;
    // float border_angle = math_consts::HALF_PI - step;
    
    // uint32_t weapon_images_quantity = weapon_->get_weapon_traits()->get_images_quantity();
    // for (uint32_t i = 0; i < weapon_images_quantity; ++i)
    // {
    //     if (OX_angle_right_semicircle >= border_angle)
    //     {
    //         bool loading_result = load_texture_from_image_manager(weapon_->get_weapon_traits()->get_image_file_name(i));
    //         assert(loading_result);
    //         calculate_scale();

    //         return;
    //     }

    //     border_angle -= 2 * step;
    // }

    // load_texture_from_image_manager(weapon_->get_weapon_traits()->get_image_file_name(weapon_images_quantity - 1));
    // calculate_scale();
}
