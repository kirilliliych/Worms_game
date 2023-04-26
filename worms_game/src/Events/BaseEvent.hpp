#pragma once

enum class EventTypes {
        NO_EVENT,
        MOUSE_BUTTON_DOWN,
        MOUSE_BUTTON_UP,
        KEYBOARD_KEY_DOWN,
        KEYBOARD_KEY_UP,
        MOUSE_MOVE,
        TIME_PASSED,
        OTHER_EVENT,
        EVENT_TYPE_CNT
    };

class BaseEvent {
  public:
  
  protected:
    EventTypes event_type_;

    BaseEvent(EventTypes event_type) : event_type_{event_type} {}
  
  public:
    EventTypes get_type() {
        return event_type_;
    }
};
