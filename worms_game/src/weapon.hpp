#pragma once


#include "abstract_node.hpp"
#include "game.hpp"
#include "map.hpp"
#include "projectile.hpp"
#include "sfmlwrap/events/event.hpp"
#include "weapon_traits.hpp"


class Weapon : public AbstractNode
{
public:

    Weapon(AbstractNode *parent, const Rect<int> &area, const WeaponTraits *w_traits) // w_traits == nullptr means character does not hold a weapon
      : AbstractNode(parent, area),
        OX_angle_(-3.14159f / 2),
        projectile_spawn_position_(area_.center()),
        w_traits_(w_traits),
        charge_level_(0),
        is_charging_(false),
        fires_(false)
    {
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

    const WeaponTraits *get_weapon_traits() const
    {
        return w_traits_;
    }

    float get_charge_level() const
    {
        return charge_level_;
    }

    void set_projectile_spawn_position(const Point2d<int> &position)
    {
        projectile_spawn_position_ = position;
    }

    void set_OX_angle(float OX_angle)
    {
        OX_angle_ = OX_angle;
    }

    void set_weapon_traits(const WeaponTraits *weapon_traits)
    {
        w_traits_ = weapon_traits;
    }

    void render_self(Surface *surface, const Point2d<int> &camera_offset) override
    {
        assert(surface != nullptr);

        // nothing?
    }

    bool handle_event(const Event &event) override
    {
        bool result = false;

        if (w_traits_ != nullptr)
        {
            switch (event.get_type())
            {
                case EventType::KEY_PRESSED:
                {
                    if (Game::game->is_under_control(parent_))
                    {
                        switch (event.kedata_.key_code)
                        {
                            case KeyboardKey::Space:
                            {
                                if (!is_charging_)  // not necessary?
                                {
                                    charge_level_ = 0;
                                }

                                is_charging_ = true;

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

                case EventType::KEY_RELEASED:
                {
                    if (Game::game->is_under_control(parent_))
                    {
                        if (is_charging_)
                        {
                            fires_ = true;
                        }

                        is_charging_ = false;
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

                case EventType::TIME_PASSED:
                {
                    if (Game::game->is_under_control(parent_))
                    {
                        if (is_charging_)
                        {
                            charge_level_ += w_traits_->get_charging_speed() * Game::game->time_delta.count();
                            if (charge_level_ >= 1.f)
                            {
                                charge_level_ = 1.f;
                                fires_ = true;
                            }
                        }

                        if (fires_)
                        {
                            // printf("projectile added\n");
                            Game::game->add_to_map_children(new Projectile(nullptr, projectile_spawn_position_,
                                                                           OX_angle_, charge_level_,
                                                                           w_traits_->get_ammo_traits()));

                            is_charging_ = false;
                            charge_level_ = 0;
                            fires_ = false;

                            w_traits_ = nullptr;
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
        }

        return result;
    }


private:

    float OX_angle_;
    Point2d<int> projectile_spawn_position_;

    const WeaponTraits *w_traits_;

    float charge_level_;
    bool is_charging_;
    bool fires_;
};

