#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP


#include "abstract_node.hpp"
#include "physics_object.hpp"
#include "sfmlwrap/texture.hpp"
#include <math.h>


class Projectile : public PhysicsObject
{
public:

    Projectile(AbstractNode *parent, const Rect<int> &area)
      : PhysicsObject(parent, area, {0, 0}, {0, 0}, 1, 1),
        damage_(20)
    {
        type_ = PhysicsEntity::PROJECTILE;
    }

    Projectile(AbstractNode *parent, const Rect<int> &area, const std::string &texture_file_name,
               const Rect<int> &texture_area = Rect<int>())
      : PhysicsObject(parent, area, {0, 0}, {0, 0}, 1, 1, texture_file_name, texture_area),
        damage_(20)
    {
        type_ = PhysicsEntity::PROJECTILE;

        uint32_t texture_width  = texture_->get_width();
        uint32_t texture_height = texture_->get_height();
        assert(area.get_width()  >= 0);
        assert(area.get_height() >= 0);
        float asked_width  = static_cast<float> (area.get_width());
        float asked_height = static_cast<float> (area.get_height());
        float x_scale = asked_width  / static_cast<float> (texture_width);
        float y_scale = asked_height / static_cast<float> (texture_height);
        if (y_scale < x_scale)
        {
            area_.set_width(y_scale * static_cast<int> (texture_width));
            texture_scale_ = y_scale;
        }
        else
        {
            area_.set_height(x_scale * static_cast<int> (texture_height));
            texture_scale_ = x_scale;
        }
    }

    void on_bounce_death(const Point2d<int> &death_position) override
    {
        Game::game->process_explosion(40, death_position);
    }

    void render_self(Surface *surface, const Point2d<int> &camera_offset) override
    {
        assert(surface != nullptr);

        Sprite self_sprite(*texture_, area_.left_top() - camera_offset);
        self_sprite.set_scale(texture_scale_, texture_scale_);
        float rad_angle = atan2f(velocity_.y(), velocity_.x());
        self_sprite.set_rotation(rad_angle * 180 / 3.14159);
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

    float texture_scale_;

    int damage_;
};

#endif
