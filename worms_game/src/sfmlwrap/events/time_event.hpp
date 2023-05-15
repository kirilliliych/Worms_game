#pragma once


#include <chrono>
#include <ratio>
#include "base_event.hpp"


// class TimeEvent : public BaseEvent
// {
//     using time_delta_t = std::chrono::duration<float, std::chrono::milliseconds::period>;

// public:

//   TimeEvent(time_delta_t dt)
//     : BaseEvent(EventType::TIME_PASSED),
//       dt_(dt)
//   {}

// public:

//     time_delta_t dt_;
// };