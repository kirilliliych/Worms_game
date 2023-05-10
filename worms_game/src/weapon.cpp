// #include "weapon.hpp"
// #include "weapon_ui.hpp"


//     Weapon::Weapon(AbstractNode *parent, const Rect<int> &area, const WeaponTraits *w_traits) // w_traits == nullptr means character does not hold a weapon
//       : AbstractNode(parent, area),
//         OX_angle_(-3.14159f / 2),
//         projectile_spawn_position_(area_.center()),
//         w_traits_(w_traits),
//         charge_level_(0),
//         is_charging_(false),
//         fires_(false),
//         weapon_ui_(new WeaponUI(this, {area.get_width(), 5, {area_.get_left_x(), area_.get_bottom_y() - 10}}))
//     {
//         calculate_scale();
//     }

//     Weapon::~Weapon()
//     {
//         delete weapon_ui_;
//     }

//     const WeaponTraits *Weapon::get_weapon_traits() const
//     {
//         return w_traits_;
//     }

//     float Weapon::get_charge_level() const
//     {
//         return charge_level_;
//     }

//     void Weapon::set_projectile_spawn_position(const Point2d<int> &position)
//     {
//         projectile_spawn_position_ = position;
//     }

//     void Weapon::set_OX_angle(float OX_angle)
//     {
//         OX_angle_ = OX_angle;
//     }

//     void Weapon::set_weapon_traits(const WeaponTraits *weapon_traits)
//     {
//         w_traits_ = weapon_traits;
//     }

//     void Weapon::render_self(Surface *surface, const Point2d<int> &camera_offset)
//     {
//         assert(surface != nullptr);
//     }

//     bool Weapon::handle_event(const Event &event)
//     {
//         bool result = false;

//         if (w_traits_ != nullptr)
//         {
//             switch (event.get_type())
//             {
//                 case EventType::KEY_PRESSED:
//                 {
//                     if (Game::game->is_under_control(parent_))
//                     {
//                         switch (event.kedata_.key_code)
//                         {
//                             case KeyboardKey::Space:
//                             {
//                                 if (!is_charging_)  // not necessary?
//                                 {
//                                     charge_level_ = 0;
//                                 }

//                                 is_charging_ = true;

//                                 break;
//                             }

//                             default:
//                             {
//                                 break;
//                             }
//                         }
//                     }

//                     if (children_handle_event(event))
//                     {
//                         result = true;
//                     }

//                     break;
//                 }

//                 case EventType::KEY_RELEASED:
//                 {
//                     if (Game::game->is_under_control(parent_))
//                     {
//                         if (is_charging_)
//                         {
//                             fires_ = true;
//                         }

//                         is_charging_ = false;
//                     }

//                     if (children_handle_event(event))
//                     {
//                         result = true;
//                     }

//                     break;
//                 }

//                 case EventType::TIME_PASSED:
//                 {
//                     if (Game::game->is_under_control(parent_))
//                     {
//                         if (is_charging_)
//                         {
//                             charge_level_ += w_traits_->get_charging_speed() * Game::game->time_delta.count();
//                             if (charge_level_ >= 1.f)
//                             {
//                                 charge_level_ = 1.f;
//                                 fires_ = true;
//                             }
//                         }

//                         if (fires_)
//                         {
//                             // printf("projectile added\n");
//                             Game::game->add_to_map_children(new Projectile(nullptr, projectile_spawn_position_,
//                                                                            OX_angle_, charge_level_,
//                                                                            w_traits_->get_ammo_traits()));

//                             is_charging_ = false;
//                             charge_level_ = 0;
//                             fires_ = false;

//                             w_traits_ = nullptr;
//                         }
//                     }

//                     if (children_handle_event(event))
//                     {
//                         result = true;
//                     }

//                     break;
//                 }


//                 default:
//                 {
//                     if (children_handle_event(event))
//                     {
//                         result = true;
//                     }
//                 }
//             }
//         }

//         return result;
//     }


