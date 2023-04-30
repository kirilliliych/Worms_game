#ifndef BASE_EVENT_HPP
#define BASE_EVENT_HPP

enum class EventTypes{
    NO_EVENT,
    MOUSE_BUTTON_CLICKED,
    MOUSE_BUTTON_RELEASED,
    MOUSE_MOVED,
    KEY_PRESSED,
    KEY_RELEASED,
    TIME_PASSED,
    QUIT_EVENT,
    EXPLOSION_EVENT,
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

#endif
