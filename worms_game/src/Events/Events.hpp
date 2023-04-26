#pragma once

#include <SFML/Window.hpp>

#include "Events/MouseEvents.hpp"
#include "Events/KeyboardEvents.hpp"
#include "Events/TimeEvent.hpp"

class NoEvent : public BaseEvent {
  public:
    NoEvent() : BaseEvent(EventTypes::NO_EVENT) {}
};

class OtherEvent : public BaseEvent {
  public:
    OtherEvent() : BaseEvent(EventTypes::OTHER_EVENT) {}
};
