#pragma once

#include <SFML/Window.hpp>

#include "Events/BaseEvent.hpp"

class TimeEvent : public BaseEvent {
  public:
    TimeEvent(float dt) : BaseEvent(EventTypes::TIME_PASSED), dt_{dt} {}

  public:
    float dt_;
};