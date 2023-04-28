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

    void operator +=(const Point2d &another_point)
    {
        x_ += another_point.x;
        y_ += another_point.y;
    }

    void operator -=(const Point2d &another_point)
    {
        x_ -= another_point.x;
        y_ -= another_point.y;
    }

    Point2d operator +(const Point2d &another_point) const
    {      
        return {this->x_ + another_point.x_,
                this->y_ + another_point.y};
    }
    
    Point2d operator -(const Point2d &another_point) const
    {
        return {this->x_ - another_point.x_,
                this->y_ - another_point.y};
    }

    Point2d operator -() const
    {
        return {-this->x_,
                -this->y_};
    }

    bool operator ==(const Point2d &another_point) const
    {
        return ((x_ == another_point.x) && (y_ == another_point.y));
    }

    bool operator !=(const Point2d &another_point) const
    {
        return !(*this == another_point);
    }

    bool operator <=(const Point2d &another_point) const
    {
        return x_ == another_point.x ? y_ <= another_point.y : x_ <= another_point.x;
    }

    bool operator <(const Point2d &another_point) const
    {
        return x_ == another_point.x ? y_ <  another_point.y : x_  < another_point.x;
    }

    bool operator >=(const Point2d &another_point) const
    {
        return x_ == another_point.x ? y_ >= another_point.y : x_ >= another_point.x;
    }

    bool operator >(const Point2d &another_point) const
    {
        return x_ == another_point.x ? y_ >  another_point.y : x_ >  another_point.x; 
    }


private:
//-----------------------------------Variables-------------------------------------
    Type x_;
    Type y_;
};

#endif