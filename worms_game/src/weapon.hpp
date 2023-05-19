#pragma once


#include "abstract_node.hpp"
#include "game.hpp"
#include "map.hpp"
#include "maths.hpp"
#include "projectile.hpp"
#include "sfmlwrap/events/event.hpp"
#include "weapon_traits.hpp"
#include "weapon_ui.hpp"


class Weapon;


class WeaponUI : public AbstractNode
{
public:

    WeaponUI(AbstractNode *parent, const Rect<int> &area);

    void render_self(Surface *surface, const Point2d<int> &camera_offset) override;

    bool handle_event(const Event &event) override;

private:

    void update_charging_line_texture_(float new_charge_level);

private:

    const Weapon *weapon_parent_;
};



class Weapon : public AbstractNode
{
public:

    Weapon(AbstractNode *parent, const Rect<int> &area, const WeaponTraits *w_traits) // w_traits == nullptr means character does not hold a weapon
      : AbstractNode(parent, area),
        OX_angle_(-math_consts::HALF_PI),
        projectile_spawn_position_(area_.center()),
        w_traits_(w_traits),
        charge_level_(0),
        is_charging_(false),
        fires_(false),
        weapon_ui_(new WeaponUI(this, {area.width(), 5, {area.left_x(), area.bottom_y() + 10}}))
    {
        calculate_scale();
    }

    ~Weapon()
    {
        delete weapon_ui_;
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
                                if (!is_charging_)
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

                    if (children_handle_event(event))
                    {
                        result = true;
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

                    if (children_handle_event(event))
                    {
                        result = true;
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
                            Projectile *spawned_projectile = new Projectile(nullptr, projectile_spawn_position_,
                                                                           OX_angle_, charge_level_,
                                                                           w_traits_->get_ammo_traits());
                            Game::game->add_to_map_children(spawned_projectile);
                            Game::game->set_camera_tracking_object(spawned_projectile);
                            // printf("projectile set as center of camera\n");

                            is_charging_ = false;
                            charge_level_ = 0;
                            fires_ = false;

                            w_traits_ = nullptr;

                            Game::game->finish_player_action();
                            Game::game->set_player_control(false);
                        }
                    }
                    area_.set_left_top(parent_->get_area().left_top());

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

    WeaponUI *weapon_ui_;
};



inline WeaponUI::WeaponUI(AbstractNode *parent, const Rect<int> &area)
  : AbstractNode(parent, area),
    weapon_parent_(dynamic_cast<const Weapon *> (parent))
{
    assert(weapon_parent_ != nullptr);
}

inline void WeaponUI::render_self(Surface *surface, const Point2d<int> &camera_offset)
{
    assert(surface != nullptr);

    float charge_level = weapon_parent_->get_charge_level();
    if (charge_level > 0)
    {
        update_charging_line_texture_(charge_level);
        
        AbstractNode::render_self(surface, camera_offset);
    }
}

inline bool WeaponUI::handle_event(const Event &event)
{
    bool result = false;

    switch (event.get_type())
    {
        case EventType::TIME_PASSED:
        {
            area_.set_left_top({weapon_parent_->get_area().left_top().x(),
                                weapon_parent_->get_area().left_bottom().y() + 10});

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

inline void WeaponUI::update_charging_line_texture_(float new_charge_level)
{
    int width  = area_.width();
    int height = area_.height();
    std::vector<uint32_t> pixels(width * height, 0);
    for (int cur_width = 0; cur_width < width * new_charge_level; ++cur_width)
    {
        for (int cur_height = 0; cur_height < height; ++cur_height)
        {
            pixels[cur_height * width + cur_width] = 0xffffffff;
        }
    }

    texture_->update(reinterpret_cast<const uint8_t *> (pixels.data()), width, height, 0, 0);
}