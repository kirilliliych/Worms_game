#pragma once

#include <SFML/Window.hpp>

#include "events/mouse_events.hpp"
#include "events/key_events.hpp"
#include "events/time_event.hpp"
#include "events/quit_event.hpp"
#include "events/explosion_event.hpp"

class NoEvent : public BaseEvent {
  public:
    NoEvent() : BaseEvent(EventTypes::NO_EVENT) {}
};

class OtherEvent : public BaseEvent {
  public:
    OtherEvent() : BaseEvent(EventTypes::OTHER_EVENT) {}
};
