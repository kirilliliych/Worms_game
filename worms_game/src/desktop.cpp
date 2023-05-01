#include "desktop.hpp"
#include "sfmlwrap/events/base_event.hpp"


void Desktop::process_events() const
{
    // looks cringe, but idk how to write this part in another way via our current events classes condition
    bool event_is_polled = true;
    // handle_event(poll_event(&event_is_polled));  // universal virtual function for handling events with switch inside?
    while (event_is_polled)
    {
        //handle_event(poll_event(&event_is_polled));
    }
}