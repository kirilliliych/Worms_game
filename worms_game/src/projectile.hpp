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

    // Projectile(AbstractNode *parent, const Rect<int> &area)
    //   : PhysicsObject(parent, area, {0, 0}, {0, 0}, 1, 1),
    //     // explosion_radius_(80),
    //     // damage_(20)
    // {
    //     type_ = PhysicsEntity::PROJECTILE;
    // }

    // Projectile(AbstractNode *parent, const Rect<int> &area, const std::string &texture_file_name,
    //            const Rect<int> &texture_area = Rect<int>())
    //   : PhysicsObject(parent, area, {0, 0}, {0, 0}, 1, 1, texture_file_name, texture_area),
    //     // explosion_radius_(80),
    //     // damage_(20)
    // {
    //     type_ = PhysicsEntity::PROJECTILE;

    //     // uint32_t texture_width  = texture_->get_width();
    //     // uint32_t texture_height = texture_->get_height();
    //     // float asked_width  = static_cast<float> (area.get_width());
    //     // float asked_height = static_cast<float> (area.get_height());
    //     // float x_scale = asked_width  / static_cast<float> (texture_width);
    //     // float y_scale = asked_height / static_cast<float> (texture_height);
    //     // if (y_scale < x_scale)
    //     // {
    //     //     area_.set_width(y_scale * static_cast<int> (texture_width));
    //     //     texture_scale_ = y_scale;
    //     // }
    //     // else
    //     // {
    //     //     area_.set_height(x_scale * static_cast<int> (texture_height));
    //     //     texture_scale_ = x_scale;
    //     // }

    //     calculate_scale();
    // }

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
        // uint32_t texture_width  = texture_->get_width();
        // uint32_t texture_height = texture_->get_height();
        // float asked_width  = static_cast<float> (a_traits.get_width());
        // float asked_height = static_cast<float> (a_traits.get_height());
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

    void on_bounce_death(const Point2d<int> &death_position) override
    {
        // Game::game->process_explosion(a_traits_->get_explosion_radius(), death_position);
        Event event;
        event.set_type(EventType::EXPLOSION_EVENT);
        event.eedata_.radius = a_traits_->get_explosion_radius();
        event.eedata_.position = death_position;
        Game::game->launch_event(event);
    }

    void render_self(Surface *surface, const Point2d<int> &camera_offset) override
    {
        assert(surface != nullptr);

        // assert(texture_->load_from_file("rocket.png"));
        // printf("RENDERING PROJECTILE\n");
        Sprite self_sprite(*texture_, area_.left_top() - camera_offset);
        self_sprite.set_scale(texture_scale_, texture_scale_);
        self_sprite.set_origin(texture_->get_center().x(), 0);
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

private:

    const AmmoTraits *a_traits_;
};

#endif
