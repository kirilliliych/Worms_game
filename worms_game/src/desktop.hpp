#pragma once


#include <SFML/Graphics.hpp>
#include "abstract_node.hpp"
#include "game.hpp"
#include "sfmlwrap/events/event.hpp"
#include "sfmlwrap/rect.hpp"
#include "sfmlwrap/surface.hpp"
#include "sfmlwrap/texture.hpp"
#include "sfmlwrap/window.hpp"


class Desktop final : public Window, public AbstractNode
{
public:

    Desktop(uint32_t width, uint32_t height, const std::string &title)
      : Window(width, height, title),
        AbstractNode(nullptr, Rect<int>(static_cast<int> (width),
                                        static_cast<int> (height),
                                        {0, 0})),
        main_surface_(new Surface())
    {
        main_surface_->create(width, height);
    }

    ~Desktop()
    {
        delete main_surface_;
    }

    void redraw(const Point2d<int> &camera_position)
    {
        clear();

        main_surface_->clear();
        render(main_surface_, camera_position);
        main_surface_->update();
        
        draw_surface(main_surface_);

        update();
    }

    bool handle_event(const Event &event) override
    {
        bool result = false;

        switch (event.get_type())
        {
            case EventType::QUIT_EVENT:
            {
                close();

                break;
            }

            default:
            {
                if (children_handle_event(event))
                {
                    result = true;
                }
            }
        }

        return result;
    }

    // void process_events()
    // {
    //     Event event;
    //     while (poll_event(&event))
    //     {
    //         // if ((event.get_type() == EventType::MOUSE_BUTTON_PRESSED)  ||
    //         //     (event.get_type() == EventType::MOUSE_BUTTON_RELEASED))
    //         // {
    //         //     event.mbedata_.position += camera_->get_position();
    //         // }
    //         // if (event.get_type() == EventType::MOUSE_MOVED)
    //         // {
    //         //     event.mmedata_.position += camera_->get_position();
    //         // }
    //         handle_event(event);
    //     }
    // }

private:

    Surface *main_surface_;
};

