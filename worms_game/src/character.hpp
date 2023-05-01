#ifndef CHARACTER_HPP
#define CHARACTER_HPP


#include "abstract_node.hpp"
#include "physics_object.hpp"
#include "sfmlwrap/texture.hpp"
#include "weapon.hpp"
#include <algorithm>


class Character : public PhysicsObject
{
public:

    Character(AbstractNode *parent, const Rect<int> &area)
      : PhysicsObject(parent, area),
        hp_(100),
        under_control_(true),
        is_alive_(true)
    {}

    Character(AbstractNode *parent, const Rect<int> &area, const std::string &texture_file_name,
              const Rect<int> &texture_area = Rect<int>())
      : PhysicsObject(parent, area, texture_file_name, texture_area),
        hp_(100),
        under_control_(true),
        is_alive_(true)
    {
        uint32_t texture_width  = texture_->get_width();
        uint32_t texture_height = texture_->get_height();
        assert(area.get_width()  >= 0);
        assert(area.get_height() >= 0);
        uint32_t asked_width  = static_cast<uint32_t> (area.get_width());
        uint32_t asked_height = static_cast<uint32_t> (area.get_height());
        float scale = std::min((static_cast<float> (asked_width))  / static_cast<float> (texture_width),
                               (static_cast<float> (asked_height)) / static_cast<float> (texture_height));
    }

private:

    // Weapon cur_weapon_;
    // Crosshair crosshair_;
public:
    int hp_;

    bool under_control_;
    bool is_alive_; // shows whether worm or grave will be drawn 
};


#endif
