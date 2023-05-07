#pragma once


#include "abstract_node.hpp"
#include "desktop.hpp"
#include "game.hpp"
#include "sfmlwrap/events/event.hpp"


class EventManager
{
public:

    EventManager(AbstractNode *root)
      : root_(root)
    {
        assert(root != nullptr);
    }
 
    void process_external_events(const Window *polling_window)
    {
        assert(polling_window != nullptr);

        Event event;
        while (polling_window->poll_event(&event))
        {
            convert_coords_(&event, false);
            handle_event(event);
            convert_coords_(&event, true);
        }
    }

    bool handle_event(const Event &event)
    {
        return root_->handle_event(event);
    }

private:

    void convert_coords_(Event *event, bool inverse)
    {
        assert(event != nullptr);

        Point2d<int> camera_pos = Game::game->get_camera_position();
        switch (event->get_type())
        {
            case EventType::MOUSE_BUTTON_PRESSED:
            {
                event->mbedata_.position = inverse ? event->mbedata_.position - camera_pos :
                                                     event->mbedata_.position + camera_pos;

                break;
            }

            case EventType::MOUSE_BUTTON_RELEASED:
            {
                event->mbedata_.position = inverse ? event->mbedata_.position - camera_pos :
                                                     event->mbedata_.position + camera_pos;

                break;
            }

            case EventType::MOUSE_MOVED:
            {
                event->mmedata_.position = inverse ? event->mmedata_.position - camera_pos :
                                                     event->mmedata_.position + camera_pos;

                break;
            }

            default:
            {}
        }
    }

private:

    AbstractNode *root_;
};