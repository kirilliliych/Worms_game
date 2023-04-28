#ifndef PHYSICS_OBJECT_HPP
#define PHYSICS_OBJECT_HPP


#include "abstract_node.hpp"
#include "point2d.hpp"
#include "sfmlwrap/rect.hpp"


class PhysicsObject : public AbstractNode
{
public:

    PhysicsObject(AbstractNode *parent)
      : AbstractNode(parent)
    {}

    PhysicsObject(AbstractNode *parent, const std::string &texture_file_name, const Rect<int> &area = Rect<int>())
      : AbstractNode(parent, texture_file_name, area)
    {}

private:

    Point2d<int> position_;
    Point2d<float> velocity_;
    Point2d<float> acceleration_;

    Rect<int> hitbox_;

    bool is_stable_;
};

#endif