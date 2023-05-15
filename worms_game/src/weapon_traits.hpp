#pragma once


#include "ammo_traits.hpp"
#include "maths.hpp"
#include "sfmlwrap/image.hpp"


enum Weapons
{
    ROCKET_LAUNCHER = 0
};


class WeaponTraits
{
public:

    WeaponTraits(const std::vector<std::string> &image_file_names, float charging_speed, const AmmoTraits &a_traits)
      : image_file_names_(image_file_names),
        charging_speed_(charging_speed),
        a_traits_(a_traits)
    {}

    // const std::vector<std::string> &get_image_file_names() const
    // {
    //     return image_file_names_;
    // }

    const std::string &get_image_file_name(uint32_t index) const
    {
        return image_file_names_[index];
    }

    float get_charging_speed() const
    {
        return charging_speed_;
    }

    const AmmoTraits *get_ammo_traits() const
    {
        return &a_traits_;
    }

private:

    const std::vector<std::string> image_file_names_;
    float charging_speed_;

    AmmoTraits a_traits_;
};


namespace traits
{
    const std::vector<WeaponTraits> weapon_traits_pool{WeaponTraits({"rocket_launcher_down.png",
                                                                                          "rocket_launcher_left_down.png",
                                                                                          "rocket_launcher_left.png",
                                                                                          "rocket_launcher_left_up.png",
                                                                                          "rocket_launcher_up.png"
                                                                                          },
                                                                        0.75f,
                                                                        AmmoTraits(20,
                                                                                    30,
                                                                                    1,
                                                                                    1,
                                                                                    60,
                                                                                    25,
                                                                                    2000.f,
                                                                                    "rocket.png",
                                                                                    math_consts::HALF_PI))
                                                      };
}
