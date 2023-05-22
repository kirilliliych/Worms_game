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
    template<typename OtherType>
    friend class Point2d;

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

    template<typename OtherType>
    void operator +=(const Point2d<OtherType> &another_point)
    {
        x_ += static_cast<Type> (another_point.x_);
        y_ += static_cast<Type> (another_point.y_);
    }

    template<typename OtherType>
    void operator -=(const Point2d<OtherType> &another_point)
    {
        x_ -= static_cast<Type> (another_point.x_);
        y_ -= static_cast<Type> (another_point.y_);
    }

    template<typename OtherType>
    Point2d operator +(const Point2d<OtherType> &another_point) const
    {      
        return {x_ + static_cast<Type> (another_point.x_),
                y_ + static_cast<Type> (another_point.y_)};
    }
    
        template<typename OtherType>
    Point2d operator -(const Point2d<OtherType> &another_point) const
    {
        return {x_ - static_cast<Type> (another_point.x_),
                y_ - static_cast<Type> (another_point.y_)};
    }

    Point2d operator -() const
    {
        return {-x_,
                -y_};
    }

    template<typename OtherType>
    bool operator ==(const Point2d<OtherType> &another_point) const
    {
        return ((x_ == static_cast<Type> (another_point.x_)) && (y_ == static_cast<Type> (another_point.y_)));
    }

    template<typename OtherType>
    bool operator !=(const Point2d<OtherType> &another_point) const
    {
        return !(*this == another_point);
    }

    template<typename OtherType>
    bool operator <=(const Point2d<OtherType> &another_point) const
    {
        return x_ == static_cast<Type> (another_point.x_) ? y_ <= static_cast<Type> (another_point.y_) : x_ <= static_cast<Type> (another_point.x_);
    }

    template<typename OtherType>
    bool operator <(const Point2d<OtherType> &another_point) const
    {
        return x_ == static_cast<Type> (another_point.x_) ? y_ <  static_cast<Type> (another_point.y_) : x_  < static_cast<Type> (another_point.x_);
    }

    template<typename OtherType>
    bool operator >=(const Point2d<OtherType> &another_point) const
    {
        return x_ == static_cast<Type> (another_point.x_) ? y_ >= static_cast<Type> (another_point.y_) : x_ >= static_cast<Type> (another_point.x_);
    }

    template<typename OtherType>
    bool operator >(const Point2d<OtherType> &another_point) const
    {
        return x_ == static_cast<Type> (another_point.x_) ? y_ >  static_cast<Type> (another_point.y_) : x_ >  static_cast<Type> (another_point.x_); 
    }


private:
//-----------------------------------Variables-------------------------------------
    Type x_;
    Type y_;
};

#endif