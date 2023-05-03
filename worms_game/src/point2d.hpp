#ifndef POINT2D_HPP
#define POINT2D_HPP


#include <cassert>
#include <cmath>
#include <cstring>
#include <cstdint>
#include <iostream>


template<typename Type>
class Point2d
{
public:
//---------------------------------------------------------------------------------
    Point2d()
      : Point2d(0, 0)
    {}

    Point2d(Type x, Type y)
      : x_(x),
        y_(y)
    {}
//---------------------------------------------------------------------------------
    Type x() const
    {
        return x_;
    }

    Type y() const
    {
        return y_;
    }

    void set_x(Type x)
    {
        x_ = x;
    }

    void set_y(Type y) 
    {
        y_ = y;
    }

    void operator +=(const Point2d &another_point)
    {
        x_ += another_point.x_;
        y_ += another_point.y_;
    }

    void operator -=(const Point2d &another_point)
    {
        x_ -= another_point.x_;
        y_ -= another_point.y_;
    }

    Point2d operator +(const Point2d &another_point) const
    {      
        return {this->x_ + another_point.x_,
                this->y_ + another_point.y_};
    }
    
    Point2d operator -(const Point2d &another_point) const
    {
        return {this->x_ - another_point.x_,
                this->y_ - another_point.y_};
    }

    Point2d operator -() const
    {
        return {-this->x_,
                -this->y_};
    }

    bool operator ==(const Point2d &another_point) const
    {
        return ((x_ == another_point.x_) && (y_ == another_point.y_));
    }

    bool operator !=(const Point2d &another_point) const
    {
        return !(*this == another_point);
    }

    bool operator <=(const Point2d &another_point) const
    {
        return x_ == another_point.x_ ? y_ <= another_point.y_ : x_ <= another_point.x_;
    }

    bool operator <(const Point2d &another_point) const
    {
        return x_ == another_point.x_ ? y_ <  another_point.y_ : x_  < another_point.x_;
    }

    bool operator >=(const Point2d &another_point) const
    {
        return x_ == another_point.x_ ? y_ >= another_point.y_ : x_ >= another_point.x_;
    }

    bool operator >(const Point2d &another_point) const
    {
        return x_ == another_point.x_ ? y_ >  another_point.y_ : x_ >  another_point.x_; 
    }


private:
//-----------------------------------Variables-------------------------------------
    Type x_;
    Type y_;
};

#endif