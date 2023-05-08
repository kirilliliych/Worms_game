#pragma once


#include "ammo_traits.hpp"


namespace weapons
{
    
}


class WeaponTraits
{
public:

    WeaponTraits(const std::string &image_file_name, float charging_speed, const AmmoTraits &a_traits)
      : texture_(new Texture()),
        charging_speed_(charging_speed),
        a_traits_(a_traits)
    {

    }

    ~WeaponTraits()
    {
        delete texture_;
    }

    float get_charging_speed() const
    {
        return charging_speed_;
    }

    const AmmoTraits &get_ammo_traits() const
    {
        return a_traits_;
    }

private:

    Texture *texture_;
    float charging_speed_;

    AmmoTraits a_traits_;
};