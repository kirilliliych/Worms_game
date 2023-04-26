#pragma once

#include <SFML/Window.hpp>

#include "Events/BaseEvent.hpp"

class QuitEvent : public BaseEvent {
  public:
    QuitEvent() : BaseEvent(EventTypes::QUIT_EVENT) {}
};

