#ifndef RECT_HPP
#define RECT_HPP


#include <SFML/Graphics/Rect.hpp>
#include <cstdint>
#include <SFML/Graphics.hpp>

#include "../point2d.hpp"


template<typename Type>
class Rect
{
public:

    Rect()
      : Rect(0, 0, {0, 0})
    {}

    Rect(Type width, Type height, Point2d<Type> left_top)
      : width_(width),
        height_(height),
        left_top_(left_top)
    {}

    Point2d<Type> left_top() const
    {
        return left_top_;
    }

    Point2d<Type> right_top() const
    {
        return {left_top_.x() + width_, left_top_.y()};
    }

    Point2d<Type> left_bottom() const
    {
        return {left_top_.x(), left_top_.y() + height_};
    }

    Point2d<Type> right_bottom() const
    {
        return {left_top_.x() + width_, left_top_.y() + height_};
    }
    
    sf::Rect<Type> to_SFML_rect() const
    {
        return {left_top_.x(), left_top_.y(), width_, height_};
    }

private:

    Type width_;
    Type height_;
    Point2d<Type> left_top_;
};

#endif
