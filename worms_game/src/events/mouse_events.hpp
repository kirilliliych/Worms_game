#pragma once

#include <SFML/Window.hpp>

#include "Events/BaseEvent.hpp"
#include "Vector2d.hpp"

class MouseButtonDownEvent : public BaseEvent {
  public:
    MouseButtonDownEvent(sf::Mouse::Button button) : BaseEvent(EventTypes::MOUSE_BUTTON_CLICKED), button_{button} {}

  public:
    sf::Mouse::Button button_;
};

class MouseButtonUpEvent : public BaseEvent {
  public:
    MouseButtonUpEvent(sf::Mouse::Button button) : BaseEvent(EventTypes::MOUSE_BUTTON_RELEASED), button_{button} {}

  public:
    sf::Mouse::Button button_;
};

class MouseMoveEvent : public BaseEvent {
  public:
    MouseMoveEvent(Vector2d pos, Vector2d direction) : BaseEvent(EventTypes::MOUSE_MOVED), pos_{pos}, direction_{direction} {}
  
  public:
    Vector2d pos_;
    Vector2d direction_;
};