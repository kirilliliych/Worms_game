#pragma once

#include <SFML/Window.hpp>

#include "base_event.hpp"


class TimeEvent : public BaseEvent
{
public:

  TimeEvent(float dt)
    : BaseEvent(EventType::TIME_PASSED),
      dt_(dt)
  {}

public:

    float dt_;
};