#ifndef PHYSICS_OBJECT_HPP
#define PHYSICS_OBJECT_HPP


#include "abstract_node.hpp"
#include "point2d.hpp"
#include "sfmlwrap/rect.hpp"


class PhysicsObject : public AbstractNode
{
protected:

    PhysicsObject(AbstractNode *parent, const Rect<int> &area)
      : AbstractNode(parent, area),
        velocity_({0, 0}),
        acceleration_({0, 0}),   // NOTE: no gravity yet
        is_stable_(true)
    {}

    PhysicsObject(AbstractNode *parent, const Rect<int> &area, const std::string &texture_file_name,
                  const Rect<int> &texture_area = Rect<int>())
      : AbstractNode(parent, area, texture_file_name, texture_area),
        velocity_({0, 0}),
        acceleration_({0, 0}),
        is_stable_(true)
    {}

protected:

    Point2d<float> velocity_;
    Point2d<float> acceleration_;

    bool is_stable_;
};

#endif