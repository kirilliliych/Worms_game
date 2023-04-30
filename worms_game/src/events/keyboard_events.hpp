#pragma once

#include <SFML/Window.hpp>

#include "Events/BaseEvent.hpp"

class KeyboardButtonDownEvent : public BaseEvent {
  public:
    KeyboardButtonDownEvent(sf::Keyboard::Key key) : BaseEvent(EventTypes::KEYBOARD_KEY_DOWN), key_{key} {}

  public:
    sf::Keyboard::Key key_;
};

class KeyboardButtonUpEvent : public BaseEvent {
  public:
    KeyboardButtonUpEvent(sf::Keyboard::Key key) : BaseEvent(EventTypes::KEYBOARD_KEY_UP), key_{key} {}

  public:
    sf::Keyboard::Key key_;
};
