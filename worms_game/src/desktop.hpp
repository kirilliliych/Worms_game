#pragma once


#include <SFML/Graphics.hpp>
#include "abstract_node.hpp"
#include "camera.hpp"
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
        main_surface_(new Surface()),
        camera_(new Camera(this, {static_cast<int> (width),
                                  static_cast<int> (height), {300, 0}}))
    {
        id = 0;
        main_surface_->create(width, height);
    }

    ~Desktop()
    {
        delete main_surface_;
    }

    // void render_self(Surface *surface, const Point2d<int> &camera_offset) override
    // {
    //     assert(surface != nullptr);
    // }

    void redraw()
    {
        clear();

        main_surface_->clear();
        render(main_surface_, camera_->get_position());
        main_surface_->update();
        
        draw_surface(main_surface_);

        update();
    }

    bool handle_event(const Event &event) override
    {
        bool result = false;

        if (event.get_type() == EventType::KEY_PRESSED)
        {
            printf("desktop event code: %d\n", event.get_type());
        }
        switch (event.get_type())
        {
            // mouse_moved
            // time_passed
            // quit_event

            default:
            {
                // printf("entered desktop handle event\n");

                for (uint32_t child_index = 0; child_index < children_.size(); ++child_index)
                {
                    result = children_[child_index]->handle_event(event);
                }
            }
        }

        return result;
    }

    void process_events()
    {
        Event event;
        while (poll_event(&event))
        {
            handle_event(event);
        }

        // bool event_is_polled = true;
        // handle_event(poll_event(&event_is_polled));
        // while (event_is_polled)
        // {
        //     handle_event(poll_event(&event_is_polled));
        // }
    }

private:

    Surface *main_surface_;

    Camera *camera_;
};

