#ifndef WEAPON_HPP
#define WEAPON_HPP


#include "abstract_node.hpp"


class Weapon : public AbstractNode
{
public:

    Weapon(AbstractNode *parent, Rect<int> area = Rect<int>())
      : AbstractNode(parent, area)
    {}

private:

    
};

#endif
