#pragma once


#include "abstract_node.hpp"
#include "character.hpp"


class CharacterUI : public AbstractNode
{
public:

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

        int hp = character_parent_->get_hp();
        if (hp > 0)
        {
            update_hp_texture_(hp);
            prev_hp_ = hp;
 
            // Sprite self_sprite(*texture_, area_.left_top() - camera_offset);
            // surface->draw_sprite(self_sprite);
            AbstractNode::render_self(surface, camera_offset);
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

                if (children_handle_event(event))
                {
                    result = true;
                }

                break;
            }

            default:
            {
                if (children_handle_event(event))
                {
                    result = true;
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
        for (int cur_width = min; cur_width < max; ++cur_width)
        {
            for (int cur_height = 0; cur_height < height; ++cur_height)
            {
                pixels[cur_height * width + cur_width] = color;
            }
        }

        texture_->update(reinterpret_cast<const uint8_t *> (pixels.data()), width, height, 0, 0);
    }

private:

    const Character *character_parent_;

    int max_hp_;
    int prev_hp_;
};
