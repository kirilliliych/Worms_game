#pragma once


#include <cassert>
#include <cmath>
#include <cstring>
#include <cstdint>
#include <iostream>


template<typename Type>
class Vector2d
{
public:
//---------------------------------------------------------------------------------
    Vector2d()
      : Vector2d(0, 0)
    {}

    Vector2d(Type x, Type y)
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

    void operator +=(const Vector2d &another_point)
    {
        x_ += another_point.x_;
        y_ += another_point.y_;
    }

    void operator -=(const Vector2d &another_point)
    {
        x_ -= another_point.x_;
        y_ -= another_point.y_;
    }

    Vector2d operator +(const Vector2d &another_point) const
    {      
        return {x_ + another_point.x_,
                y_ + another_point.y_
               };
    }
    
    Vector2d operator -(const Vector2d &another_point) const
    {
        return {x_ - another_point.x_,
                y_ - another_point.y_
               };
    }

    Vector2d operator -() const
    {
        return {-x_, -y_};
    }

    Vector2d operator *(Type scalar) const
    {
        return {x_ * scalar, y_ * scalar};
    }

    bool operator ==(const Vector2d &another_point) const
    {
        return ((x_ == another_point.x_) && (y_ == another_point.y_));
    }

    bool operator !=(const Vector2d &another_point) const
    {
        return !(*this == another_point);
    }

    bool operator <=(const Vector2d &another_point) const
    {
        return x_ == another_point.x_ ? y_ <= another_point.y_ : x_ <= another_point.x_;
    }

    bool operator <(const Vector2d &another_point) const
    {
        return x_ == another_point.x_ ? y_ <  another_point.y_ : x_  < another_point.x_;
    }

    bool operator >=(const Vector2d &another_point) const
    {
        return x_ == another_point.x_ ? y_ >= another_point.y_ : x_ >= another_point.x_;
    }

    bool operator >(const Vector2d &another_point) const
    {
        return x_ == another_point.x_ ? y_ >  another_point.y_ : x_ >  another_point.x_; 
    }


private:
//-----------------------------------Variables-------------------------------------
    Type x_;
    Type y_;
};
