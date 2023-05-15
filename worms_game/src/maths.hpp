#pragma once


#include <cmath>


namespace math_consts
{
    const float PI = acosf(-1.f);
    const float HALF_PI = acosf(0.f);
    const float PI_RADIANS_DEGREES = 180.f;
    const float HALF_PI_RADIANS_DEGREES = 90.f;
}

inline float radians_to_degrees(float radians)
{
    return radians / math_consts::PI * math_consts::PI_RADIANS_DEGREES;
}
