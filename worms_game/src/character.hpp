#pragma once


#include "abstract_node.hpp"
#include "physics_object.hpp"
#include "sfmlwrap/texture.hpp"
#include "weapon.hpp"
#include <math.h>


class Character : public PhysicsObject
{
public:

    Character(AbstractNode *parent, const Rect<int> &area)    // do we need?
      : PhysicsObject(parent, area),
        texture_scale_(1),
        hp_(100),
        under_control_(true),
        is_alive_(true)
    {}

    Character(AbstractNode *parent, const Rect<int> &area, const std::string &texture_file_name,
              const Rect<int> &texture_area = Rect<int>())
      : PhysicsObject(parent, area, texture_file_name, texture_area),
        texture_scale_(1),
        hp_(100),
        under_control_(true),
        is_alive_(true)
    {
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
        }
    }

    void render_self(Surface *surface) override
    {
        assert(surface != nullptr);

        Sprite self_sprite(*texture_, area_.left_top());
        self_sprite.set_scale(texture_scale_, texture_scale_);
        surface->draw_sprite(self_sprite);
    }

private:

    float texture_scale_;

    // Weapon cur_weapon_;
    // Crosshair crosshair_;
public:
    int hp_;

    bool under_control_;
    bool is_alive_; // shows whether worm or grave will be drawn 
};
