#pragma once


#include "abstract_node.hpp"
#include "base_ui.hpp"
#include "character.hpp"

class CharacterUI : public BaseUI
{
public:

    CharacterUI(AbstractNode *parent, const Rect<int> &area, int max_value, uint32_t color)
      : BaseUI(parent, area, max_value, color),
        character_parent_(dynamic_cast<const Character *> (parent))
    {
        assert(character_parent_ != nullptr);
    }

    void render_self(Surface *surface, const Point2d<int> &camera_offset) override
    {
        assert(surface != nullptr);

        int value = character_parent_->get_hp();
        if (value > 0)
        {
            update_value_texture_(value);
            prev_value_ = value;

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

    const Character *character_parent_;
};
