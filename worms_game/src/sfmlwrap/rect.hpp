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
        return {left_top_.x() + width_ - 1, left_top_.y()};
    }

    Point2d<Type> left_bottom() const
    {
        return {left_top_.x(), left_top_.y() + height_ - 1};
    }

    Point2d<Type> right_bottom() const
    {
        return {left_top_.x() + width_ - 1, left_top_.y() + height_ - 1};
    }

    Point2d<Type> center() const
    {
        return {left_top_.x() + (width_  - 1) / 2,
                left_top_.y() + (height_ - 1) / 2
               };
    }

    Point2d<Type> half_size() const
    {
        return {width_ / 2, height_ / 2};
    }

    Point2d<Type> size() const
    {
        return {width_, height_};
    }

    Type center_x() const
    {
        return center().x();
    }

    Type center_y() const
    {
        return center().y();
    }

    Type left_x() const
    {
        return left_top_.x();
    }

    Type right_x() const
    {
        return right_bottom().x();
    }

    Type top_y() const
    {
        return right_top().y();
    }

    Type bottom_y() const
    {
        return right_bottom().y();
    }

    Type width() const
    {
        return width_;
    }

    Type height() const
    {
        return height_;
    }

    Type get_radius() const
    {
        return static_cast<Type> (sqrtf(width_ * width_ + height_ * height_));
    }

    void set_width(Type width)
    {
        width_ = width;
    }

    void set_height(Type height)
    {
        height_ = height;
    }

    void set_left_top(Point2d<Type> position)
    {
        left_top_ = position;
    }

    void set_left_top_x(int x)
    {
        left_top_.set_x(x);
    }

    void set_left_top_y(int y)
    {
        left_top_.set_y(y);
    }

    bool contains(const Point2d<Type> &position) const
    {
        return (left_top_.x() <= position.x()) && (position.x() < left_top_.x() + width_) &&
               (left_top_.y() <= position.y()) && (position.y() < left_top_.y() + height_);
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
