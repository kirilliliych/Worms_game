// #pragma once


// #include "abstract_node.hpp"
// #include "weapon.hpp"


// class Weapon;


// class WeaponUI : public AbstractNode
// {
// public:

//     WeaponUI(AbstractNode *parent, const Rect<int> &area)
//       : AbstractNode(parent, area),
//         weapon_parent_(dynamic_cast<const Weapon *> (parent))
//     {
//         assert(weapon_parent_ != nullptr);
//     }

//     void render_self(Surface *surface, const Point2d<int> &camera_offset) override;
//     // {
//         // assert(surface != nullptr);

//         // float charge_level = weapon_parent_->get_charge_level();
//         // if (charge_level > 0)
//         // {
//             // update_charging_line_texture_(charge_level);

//             // AbstractNode::render_self(surface, camera_offset);
//         // }
//     // }

//     bool handle_event(const Event &event) override;
//     // {
//         // bool result = false;

//         // switch (event.get_type())
//         // {
//             // case EventType::TIME_PASSED:
//             // {
//                 // area_.set_left_top({weapon_parent_->get_area().left_top().x(),
//                                     // weapon_parent_->get_area().left_bottom().y() + 10});

//                 // if (children_handle_event(event))
//                 // {
//                     // result = true;
//                 // }

//                 // break;
//             // }

//             // default:
//             // {
//                 // if (children_handle_event(event))
//                 // {
//                     // result = true;
//                 // }

//                 // break;
//             // }
//         // }

//         // return result;
//     // }

// private:

//     void update_charging_line_texture_(float new_charge_level);
//     // {
//         // int width  = area_.get_width();
//         // int height = area_.get_height();
//         // std::vector<uint32_t> pixels(width * height, 0);
//         // for (int cur_width = 0; cur_width < width * new_charge_level; ++cur_width)
//         // {
//             // for (int cur_height = 0; cur_height < height; ++cur_height)
//             // {
//                 // pixels[cur_height * width + cur_width] = 0xffffffff;
//             // }
//         // }

//         // texture_->update(reinterpret_cast<const uint8_t *> (pixels.data()), width, height, 0, 0);
//     // }

// private:

//     const Weapon *weapon_parent_;
// };
