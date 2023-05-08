#pragma once


#include "sfmlwrap/texture.hpp"


class AmmoTraits
{
public:

    AmmoTraits(uint32_t width, uint32_t height, int bounces_before_death, float friction, float damage,
               float abs_init_speed, const std::string &image_file_name)
      : width_(width),
        height_(height),
        bounces_before_death_(bounces_before_death),
        friction_(friction),
        damage_(damage),
        abs_init_speed(abs_init_speed),
        image_file_name_(image_file_name)
    {}

    AmmoTraits(const AmmoTraits &other) = default;


    int get_width() const
    {
        return width_;
    }

    int get_height() const
    {
        return height_;
    }

    int get_bounces_before_death() const
    {
        return bounces_before_death_;
    }

    float get_friction() const
    {
        return friction_;
    }

    float get_damage() const
    {
        return damage_;
    }

    float get_abs_init_speed() const
    {
        return abs_init_speed;
    }

    const std::string &get_image_file_name() const
    {
        return image_file_name_;
    }

private:

    int width_;
    int height_;

    int bounces_before_death_;
    float friction_;

    float damage_;
    float abs_init_speed;
    const std::string image_file_name_;
};
