#ifndef CHARACTER_HPP
#define CHARACTER_HPP


#include "physics_object.hpp"
#include "weapon.hpp"


class Character : public PhysicsObject
{
public:

private:

    Weapon cur_weapon_;
    Crosshair crosshair_;

    int hp_;                // int?

    bool is_alive_; // shows whether worm or grave will be drawn   
}


#endif
