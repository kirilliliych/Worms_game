#pragma once


#include "sfmlwrap/texture.hpp"


class AmmoTraits
{
public:

    AmmoTraits(uint32_t width, uint32_t height, int bounces_before_death, float friction, int explosion_radius, int full_damage_radius,
               float damage, float abs_init_speed, const std::string &image_file_name, float texture_original_rotation)
      : width_(width),
        height_(height),
        bounces_before_death_(bounces_before_death),
        friction_(friction),
        full_damage_radius_(full_damage_radius),
        explosion_radius_(explosion_radius),
        damage_(damage),
        abs_init_speed(abs_init_speed),
        image_file_name_(image_file_name),
        texture_original_rotation_(texture_original_rotation)
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

    int get_full_damage_radius() const
    {
        return full_damage_radius_;
    }

    int get_explosion_radius() const
    {
        return explosion_radius_;
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

    float get_texture_original_rotation() const
    {
        return texture_original_rotation_;
    }

private:

    int width_;
    int height_;

    int bounces_before_death_;
    float friction_;

    int full_damage_radius_;
    int explosion_radius_;
    float damage_;
    float abs_init_speed;

    const std::string image_file_name_;
    float texture_original_rotation_;
};
