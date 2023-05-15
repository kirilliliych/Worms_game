#pragma once


#include "base_event.hpp"
#include "point2d.hpp"


// enum class MouseButton
// {
//     Left,
//     Right,
//     Wheel,
//     Extra1,
//     Extra2,
//     Unknown
// };


// class MouseClickedEvent : public BaseEvent
// {
// public:

//     MouseClickedEvent(MouseButton button, const Point2d<int> &position)
//       : BaseEvent(EventType::MOUSE_BUTTON_CLICKED),
//         button_(button),
//         position_(position)
//     {}

// public:

//     MouseButton button_;
//     Point2d<int> position_;
// };

// class MouseReleasedEvent : public BaseEvent
// {
// public:

//     MouseReleasedEvent(MouseButton button, const Point2d<int> &position)
//       : BaseEvent(EventType::MOUSE_BUTTON_RELEASED),
//         button_(button),
//         position_(position)
//     {}

// public:
    
//     MouseButton button_;
//     Point2d<int> position_;
// };

// class MouseMovedEvent : public BaseEvent
// {
// public:

//     MouseMovedEvent(const Point2d<int> &position)
//       : BaseEvent(EventType::MOUSE_MOVED),
//         position_(position)
//     {}
  
// public:

//     Point2d<int> position_;
// };