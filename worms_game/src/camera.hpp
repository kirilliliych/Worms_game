#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "point2d.hpp"


class Camera
{
public:

    Camera(int width, int height)
      : width_(width),
        height_(height),
        move_speed_(DEFAULT_MOVE_SPEED),
        left_top_({})
    {}
    
    void set_move_speed(float camera_move_speed)
    {
        move_speed_ = camera_move_speed;
    }

private:

    static constexpr float DEFAULT_MOVE_SPEED = 400.f;

    int width_;
    int height_;
    float move_speed_;
    Point2d<float> left_top_;
};

#endif
