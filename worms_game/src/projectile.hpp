#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP


#include "abstract_node.hpp"
#include "ammo_traits.hpp"
#include "maths.hpp"
#include "physics_object.hpp"
#include "sfmlwrap/texture.hpp"


class Projectile : public PhysicsObject
{
public:

    Projectile(AbstractNode *parent, const Point2d<int> &spawn_position, float OX_angle, float charge_level, const AmmoTraits *a_traits)
      : PhysicsObject(parent, {a_traits->get_width(),
                               a_traits->get_height(),
                               spawn_position
                              },
                      {a_traits->get_abs_init_speed() * cosf(OX_angle) * charge_level,
                                 a_traits->get_abs_init_speed() * sinf(OX_angle) * charge_level},
                      {0, 0},
                      a_traits->get_friction(),
                      a_traits->get_bounces_before_death(),
                      a_traits->get_image_file_name()),
                      a_traits_(a_traits)
    {
        calculate_scale();
    }

    void on_bounce_death(const Point2d<int> &death_position) override
    {
        Event event;
        event.set_type(EventType::EXPLOSION_EVENT);
        event.eedata_.radius = a_traits_->get_explosion_radius();
        event.eedata_.position = death_position;
        event.eedata_.damage = a_traits_->get_damage();
        event.eedata_.full_damage_radius = a_traits_->get_full_damage_radius();
        Game::game->launch_event(event);
    }

    void render_self(Surface *surface, const Point2d<int> &camera_offset) override
    {
        assert(surface != nullptr);

        Sprite self_sprite(*texture_, area_.left_top() - camera_offset + texture_->get_center());
        self_sprite.set_scale(texture_scale_, texture_scale_);
        self_sprite.set_origin(texture_->get_center().x(), texture_->get_center().y());
        self_sprite.set_rotation(radians_to_degrees(a_traits_->get_texture_original_rotation() + atan2f(velocity_.y(), velocity_.x())));
        surface->draw_sprite(self_sprite);
    }

    bool handle_event(const Event &event) override
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

private:

    const AmmoTraits *a_traits_;
};

#endif
