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

    Character(AbstractNode *parent, const Rect<int> &area, const std::string &texture_file_name,
              const Rect<int> &texture_area = Rect<int>());

    ~Character();

    int get_hp() const;

    void on_bounce_death(const Point2d<int> &death_position) override;

    void render_self(Surface *surface, const Point2d<int> &camera_offset) override;

    bool handle_event(const Event &event) override;

private:

    void set_texture_by_angle_(float OX_angle);

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
 
        texture_->fill_with_color(0xff0000ff);
    }
 
    void render_self(Surface *surface, const Point2d<int> &camera_offset) override
    {
        assert(surface != nullptr);

        // printf("character UI: render_self called\n");
        int hp = character_parent_->get_hp();
        if (hp > 0)
        {
            update_hp_texture_(hp);
            prev_hp_ = hp;
 
            Sprite self_sprite(*texture_, area_.left_top() - camera_offset);
            surface->draw_sprite(self_sprite);
        }
    }
 
    bool handle_event(const Event &event) override
    {
        bool result = false;

        switch (event.get_type())
        {
            case EventType::TIME_PASSED:
            {
                area_.set_left_top({character_parent_->get_area().left_top().x(),
                                    character_parent_->get_area().left_top().y() - 10});

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

                break;
            }
        }

        return result;
    }
 
private:
 
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

private:

    const Character *character_parent_;

    int max_hp_;
    int prev_hp_;
};
