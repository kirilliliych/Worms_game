#pragma once

#include <SFML/Window.hpp>

#include "Events/BaseEvent.hpp"
#include "Vector2d.hpp"

class ExplosionEvent : public BaseEvent {
  public:
    ExplosionEvent(Vector2d pos, float r, float force) : BaseEvent(EventTypes::EXPLOSION_EVENT),  pos_{pos}, r_{r}, force_{force} {}
  
  public:
    Vector2d pos_;
    float r_;
    float force_;
};

