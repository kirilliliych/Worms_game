#pragma once


#include "abstract_node.hpp"
#include "collidable.hpp"
#include "physics_object.hpp"
#include "sfmlwrap/events/event.hpp"
#include "sfmlwrap/texture.hpp"
#include "weapon.hpp"


class Character : public PhysicsObject, public Collidable
{
public:

    Character(AbstractNode *parent, const Rect<int> &area)    // do we need this?
      : PhysicsObject(parent, area, {0, 100}, {0, 0}, DEFAULT_FRICTION),
        Collidable(CollidableEntity::CHARACTER),
        texture_scale_(1),
        hp_(100),
        under_control_(true),
        is_alive_(true)
    {
        PhysicsObject::type_ = PhysicsEntity::CHARACTER;
    }

    Character(AbstractNode *parent, const Rect<int> &area, const std::string &texture_file_name,
              const Rect<int> &texture_area = Rect<int>())
      : PhysicsObject(parent, area, {0, 100}, {0, 0},
                      DEFAULT_FRICTION, texture_file_name, texture_area),
        Collidable(CollidableEntity::CHARACTER),
        texture_scale_(1),
        hp_(100),
        under_control_(true),
        is_alive_(true)
    {
        PhysicsObject::type_ = PhysicsEntity::CHARACTER;

        uint32_t texture_width  = texture_->get_width();
        uint32_t texture_height = texture_->get_height();
        assert(area.get_width()  >= 0);
        assert(area.get_height() >= 0);
        float asked_width  = static_cast<float> (area.get_width());
        float asked_height = static_cast<float> (area.get_height());
        float x_scale = asked_width  / static_cast<float> (texture_width);
        float y_scale = asked_height / static_cast<float> (texture_height);
        if (y_scale < x_scale)
        {
            area_.set_width(y_scale * static_cast<int> (texture_width));
            texture_scale_ = y_scale;
        }
        else
        {
            area_.set_height(x_scale * static_cast<int> (texture_height));
        }
    }

    void render_self(Surface *surface, const Point2d<int> &camera_offset) override
    {
        assert(surface != nullptr);

        Sprite self_sprite(*texture_, area_.left_top() - camera_offset);
        self_sprite.set_scale(texture_scale_, texture_scale_);
        surface->draw_sprite(self_sprite);
    }

    bool handle_event(const Event &event) override
    {
        bool result = false;

        switch (event.get_type())
        {            
            // case EventType::KEY_PRESSED:
            // {
            //     const KeyPressedEvent &kpevent = reinterpret_cast<const KeyPressedEvent &> (event);
            //     switch (kpevent.code_)
            //     {
            //         case KeyboardKey::A:    // how to climb? 
            //         {
            //             printf("character key_pressed a\n");
                        
            //             if (under_control_)
            //             {
            //                 velocity_.set_x(-228);
            //             }

            //             for (uint32_t child_index = 0; child_index < children_.size(); ++child_index)
            //             {
            //                 result = children_[child_index]->handle_event(event);
            //             }

            //             break;
            //         }
                    
            //         case KeyboardKey::D:
            //         {
            //             printf("character key_pressed d\n");

            //             if (under_control_)
            //             {
            //                 velocity_.set_x(228);
            //             }

            //             for (uint32_t child_index = 0; child_index < children_.size(); ++child_index)
            //             {
            //                 result = children_[child_index]->handle_event(event);
            //             }

            //             break;
            //         }

            //         case KeyboardKey::Enter:
            //         {
            //             printf("character key_pressed enter\n");

            //             for (uint32_t child_index = 0; child_index < children_.size(); ++child_index)
            //             {
            //                 result = children_[child_index]->handle_event(event);
            //             }

            //             break;
            //         }

            //         default:
            //         {
            //             break;
            //         }
            //     }

            //     break;
            // }

            // case EventType::KEY_RELEASED:
            // {
            //     const KeyReleasedEvent &krevent = reinterpret_cast<const KeyReleasedEvent &> (event);
            //     switch (krevent.code_)
            //     {
            //         case KeyboardKey::A:
            //         {
            //             printf("character key_released a\n");
            //             if (under_control_)
            //             {
            //                 if (velocity_.x() < 0)
            //                 {
            //                     velocity_.set_x(0);
            //                 }
            //             }

            //             for (uint32_t child_index = 0; child_index < children_.size(); ++child_index)
            //             {
            //                 result = children_[child_index]->handle_event(event);
            //             }
                        
            //             break;
            //         }

            //         case KeyboardKey::D:
            //         {
            //             printf("character key_released d\n");
            //             if (under_control_)
            //             {
            //                 if (velocity_.x() > 0)
            //                 {
            //                     velocity_.set_x(0);
            //                 }
            //             }

            //             for (uint32_t child_index = 0; child_index < children_.size(); ++child_index)
            //             {
            //                 result = children_[child_index]->handle_event(event);
            //             }

            //             break;
            //         }

            //         case KeyboardKey::Enter:
            //         {
            //             printf("character key_released enter\n");

            //             for (uint32_t child_index = 0; child_index < children_.size(); ++child_index)
            //             {
            //                 result = children_[child_index]->handle_event(event);
            //             }

            //             break;
            //         }

            //         default:
            //         {
            //             break;
            //         }
            //     }

            //     break;
            // }

            case EventType::TIME_PASSED:
            {
                if (PhysicsObject::handle_event(event))
                {
                    result = true;
                }

                for (uint32_t child_index = 0; child_index < children_.size(); ++child_index)
                {
                    if (children_[child_index]->handle_event(event))
                    {
                        result = true;
                    }
                }

                break;
            }

            default:
            {
                for (uint32_t child_index = 0; child_index < children_.size(); ++child_index)
                {
                    if (children_[child_index]->handle_event(event))
                    {
                        result = true;
                    }
                }
            }
        }

        return result;
    }

private:
public:

    static constexpr float DEFAULT_FRICTION = 0.00003;

    float texture_scale_;

    // Weapon cur_weapon_;
    // Crosshair crosshair_;

    int hp_;

    bool under_control_;
    bool is_alive_; // shows whether worm or grave will be drawn 
};
