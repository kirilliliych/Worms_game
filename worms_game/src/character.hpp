#pragma once


#include "abstract_node.hpp"
#include "crosshair.hpp"
#include "debris.hpp"
#include "physics_entity.hpp"
#include "physics_object.hpp"
#include "sfmlwrap/events/event.hpp"
#include "sfmlwrap/texture.hpp"
#include "weapon.hpp"


class CharacterUI;

class Character : public PhysicsObject
{
public:

    Character(AbstractNode *parent, const Rect<int> &area);
    //   : PhysicsObject(parent, area, {0, 0}, {0, 0}, DEFAULT_FRICTION, -1),
        // crosshair_(new Crosshair(this, {10, 10, area_.center()},
                                //  std::max(area.get_width(), area_.get_height()) + 30)),
        // weapon_(new Weapon(this, area_, nullptr)),
        // hp_(100),
        // ui_(new CharacterUI(this, {area.get_width(), 5, {area.get_left_x(), area.get_top_y() - 10}}, hp_))
    // {
        // PhysicsObject::type_ = PhysicsEntity::CHARACTER;
    // }

    Character(AbstractNode *parent, const Rect<int> &area, const std::string &texture_file_name,
              const Rect<int> &texture_area = Rect<int>());
    //   : PhysicsObject(parent, area, {0, 0}, {0, 0},
                    //   DEFAULT_FRICTION, -1, texture_file_name, texture_area),
        // crosshair_(new Crosshair(this, {10, 10, area_.center()},
                                //  std::max(area.get_width(), area_.get_height()) + 30)),
        // weapon_(new Weapon(this, area_, nullptr)),
        // hp_(100),
        // ui_(new CharacterUI(this, {area.get_width(), 5, {area.get_left_x(), area.get_top_y() - 10}}, hp_))
    // {
        // PhysicsObject::type_ = PhysicsEntity::CHARACTER;
// 
        // calculate_scale();
    // }

    ~Character();
    // {
        // delete crosshair_;
        // delete weapon_;

        // delete ui_;
    // }

    int get_hp() const;
    // {
        // return hp_;
    // }

    void on_bounce_death(const Point2d<int> &death_position) override;
    // {}

    void render_self(Surface *surface, const Point2d<int> &camera_offset) override;
    // {
        // assert(surface != nullptr);

        // Sprite self_sprite(*texture_, area_.left_top() - camera_offset);
        // self_sprite.set_scale(crosshair_->is_right_semicircle() ? -texture_scale_ : texture_scale_, texture_scale_);
        // self_sprite.set_origin(texture_->get_center().x(), 0);
        // surface->draw_sprite(self_sprite);
    // }

    bool handle_event(const Event &event) override;
    // {
        // bool result = false;

        // switch (event.get_type())
        // {
            // case EventType::KEY_PRESSED:
            // {
                // if (Game::game->is_under_control(this) && 
                    // (is_stable_))
                // {
                    // switch (event.kedata_.key_code)
                    // {
                        // case KeyboardKey::Enter:
                        // {
                            // velocity_.set_x(400.0f * cosf(crosshair_->get_angle()));
                            // velocity_.set_y(400.0f * sinf(crosshair_->get_angle()));
                            // is_stable_ = false;

                            // break;
                        // }

                        // case KeyboardKey::Num0:
                        // {
                            // weapon_->set_weapon_traits(nullptr);

                            // break;
                        // }

                        // case KeyboardKey::Num1:
                        // {
                            // printf("changing weapon to rocket launcher\n");
                            // weapon_->set_weapon_traits(&traits::weapon_traits_pool[Weapons::ROCKET_LAUNCHER]);

                            // break;
                        // }

                        // default:
                        // {
                            // break;
                        // }
                    // }
                // }

                // for (uint32_t child_index = 0; child_index < children_.size(); ++child_index)
                // {
                    // if (children_[child_index]->handle_event(event))
                    // {
                        // result = true;
                    // }
                // }

                // break;
            // }

            // case EventType::COLLISION_EVENT:
            // {
                // if ((event.cedata_.checker == PhysicsEntity::PROJECTILE) && 
                    // (area_.contains(event.cedata_.position)))
                // {
                    // result = true;
                // }

                // for (uint32_t child_index = 0; child_index < children_.size(); ++child_index)
                // {
                    // if (children_[child_index]->handle_event(event))
                    // {
                        // result = true;
                    // }
                // }

                // break;
            // }

            // case EventType::EXPLOSION_EVENT:
            // {
                // float center_x = area_.center_x();
                // float center_y = area_.center_y();
                // float dx = center_x - event.eedata_.position.x();
                // float dy = center_y - event.eedata_.position.y();
                // int dx_sign = dx >= 0 ? dx > 0 ? 1 : 0 : -1;
                // float distance = sqrtf(dx * dx + dy * dy);

                // if (distance < event.eedata_.radius)
                // {   
                    // float new_x_abs_velocity = (1000.f * (1 - distance / event.eedata_.radius));
                    // float new_y_abs_velocity = (1000.f * (1 - distance / event.eedata_.radius));
                    // velocity_.set_x(new_x_abs_velocity * dx_sign);
                    // velocity_.set_y(-new_y_abs_velocity);

                    // is_stable_ = false;
                // }

                // if (PhysicsObject::handle_event(event))
                // {
                    // result = true;
                // }

                // for (uint32_t child_index = 0; child_index < children_.size(); ++child_index)
                // {
                    // if (children_[child_index]->handle_event(event))
                    // {
                        // result = true;
                    // }
                // }

                // break;
            // }

            // case EventType::TIME_PASSED:
            // {
                // if (PhysicsObject::handle_event(event))
                // {
                    // result = true;
                // }

                // weapon_->set_projectile_spawn_position(crosshair_->get_area().left_top());
                // weapon_->set_OX_angle(crosshair_->get_angle());

                // set_texture_by_angle_(crosshair_->get_angle());

                // for (uint32_t child_index = 0; child_index < children_.size(); ++child_index)
                // {
                    // if (children_[child_index]->handle_event(event))
                    // {
                        // result = true;
                    // }
                // }

                // break;
            // }

            // default:
            // {
                // for (uint32_t child_index = 0; child_index < children_.size(); ++child_index)
                // {
                    // if (children_[child_index]->handle_event(event))
                    // {
                        // result = true;
                    // }
                // }
            // }
        // }

        // return result;
    // }

private:

    void set_texture_by_angle_(float OX_angle);
    // {
        // if (weapon_->get_weapon_traits() == nullptr)
        // {
            // load_texture_from_image_manager("standing.png");
            // calculate_scale();

            // return;
        // }

        // float OX_angle_right_semicircle = OX_angle;
        // if (OX_angle < -3.14159f / 2)
        // {
            // OX_angle_right_semicircle = -3.14159f - OX_angle;
        // }
        // else if (OX_angle > 3.14159f / 2)
        // {
            // OX_angle_right_semicircle = 3.14159f - OX_angle;
        // }

        // float step = 3.14159f / 8;
        // float border_angle = 3.14159f / 2 - step;
        // for (uint32_t i = 0; i < 4; ++i)
        // {
            // if (OX_angle_right_semicircle >= border_angle)
            // {
                // assert(load_texture_from_image_manager(weapon_->get_weapon_traits()->get_image_file_name(i)));

                // return;
            // }

            // border_angle -= 2 * step;
        // }

        // load_texture_from_image_manager(weapon_->get_weapon_traits()->get_image_file_name(4));
        // calculate_scale();
    // }

private:
public:

    static constexpr float DEFAULT_FRICTION = 0.000002;

    Crosshair *crosshair_;    
    Weapon *weapon_;

    int hp_;
    CharacterUI *ui_; 
};


class CharacterUI : public AbstractNode
{
public:
// 
    CharacterUI(AbstractNode *parent, const Rect<int> &area, int max_hp)
      : AbstractNode(parent, area),
        character_parent_(dynamic_cast<const Character *> (parent)),
        max_hp_(max_hp)
    {
        assert(character_parent_ != nullptr);
// 
        // fill texture with color
    }
// 
    void render_self(Surface *surface, const Point2d<int> &camera_offset) override
    {
        assert(surface != nullptr);
// 
        // printf("character UI: render_self called\n");
        int hp = character_parent_->get_hp();
        if (hp > 0)
        {
            update_hp_texture_(hp);
            prev_hp_ = hp;
// 
            Sprite self_sprite(*texture_, area_.left_top() - camera_offset);
            surface->draw_sprite(self_sprite);
        }
    }
// 
    // bool handle_event
// 
private:
// 
    void update_hp_texture_(int new_hp)
    {
        int delta = new_hp - prev_hp_;
        if (delta == 0)
        {
            return;
        }

        int width  = area_.get_width();
        int height = area_.get_height();
        int min = width * std::min(new_hp, prev_hp_) / max_hp_;
        int max = width * std::max(new_hp, prev_hp_) / max_hp_;
        uint32_t color = delta > 0 ? 0xff0000ff : 0x00000000;
        std::vector<uint32_t> pixels(width * height, 0);
        uint32_t *pixels_data = pixels.data();
        for (int cur_width = min; cur_width < max; ++cur_width)
        {
            for (int cur_height = 0; cur_height < height; ++cur_height)
            {
                pixels[cur_height * width + cur_width] = color;
            }
        }
    }
// 
private:
// 
    const Character *character_parent_;
// 
    int max_hp_;
    int prev_hp_;
};
