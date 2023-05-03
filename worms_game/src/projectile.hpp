#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP


#include "abstract_node.hpp"
#include "physics_object.hpp"


class Projectile : public PhysicsObject
{
public:

    Projectile(AbstractNode *parent, const Rect<int> &area)
      : PhysicsObject(parent, area, {0, 0}, {0, 0})
    {
        type_ = PhysicsEntity::PROJECTILE;
    }

    Projectile(AbstractNode *parent, const Rect<int> &area, const std::string &texture_file_name,
               const Rect<int> &texture_area = Rect<int>())
      : PhysicsObject(parent, area, {0, 0}, {0, 0}, texture_file_name, area)
    {
        type_ = PhysicsEntity::PROJECTILE;
    }

    bool handle_event(const Event &event) override
    {
        bool result = false;

        switch (event.get_type())
        {
            // nothing or explosion_event ? 

            default:
            {
                for (uint32_t child_index = 0; child_index < children_.size(); ++child_index)
                {
                    result = children_[child_index]->handle_event(event);
                }

                break;
            }
        }

        return result;
    }

private:

    //?
};

#endif
