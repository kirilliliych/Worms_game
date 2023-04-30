#ifndef CHARACTER_HPP
#define CHARACTER_HPP


#include "abstract_node.hpp"
#include "physics_object.hpp"
#include "weapon.hpp"
#include <SFML/Graphics/RectangleShape.hpp>


class Character : public PhysicsObject
{
public:

    Character(AbstractNode *parent, const Rect<int> &area)
      : PhysicsObject(parent, area)
    {}

    Character(AbstractNode *parent, const Rect<int> &area, const std::string &texture_file_name,
              const Rect<int> &texture_area = Rect<int>())
      : PhysicsObject(parent, area, texture_file_name, texture_area)
    {}

private:

    // Weapon cur_weapon_;
    // Crosshair crosshair_;

    int hp_;                // int?

    bool under_control_;
    bool is_alive_; // shows whether worm or grave will be drawn 
};


#endif
