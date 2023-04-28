#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP


#include "abstract_node.hpp"
#include "physics_object.hpp"


class Projectile : public PhysicsObject
{
public:

    Projectile(AbstractNode *parent)
      : PhysicsObject(parent)
    {}

    Projectile(AbstractNode *parent, const std::string &texture_file_name, const Rect<int> &area = Rect<int>())
      : PhysicsObject(parent, texture_file_name, area)
    {}

private:

    //?
};

#endif
