#pragma once


enum class CollidableEntity
{
    MAP,
    CHARACTER
};


class Collidable
{
public:

    Collidable(CollidableEntity type)
      : type_(type)
    {}

    CollidableEntity get_collidable_type() const
    {
        return type_;
    }

private:

    CollidableEntity type_;
};
