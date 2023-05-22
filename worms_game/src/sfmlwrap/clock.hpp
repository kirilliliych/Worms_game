#pragma once


#include <SFML/System.hpp>
#include "time.hpp"


class Clock
{
    class Clock_ : public sf::Clock
    {};

public:

    Clock()
      : clock_(new Clock_())
    {}

    Time get_elapsed_time() const
    {
        return clock_->getElapsedTime();
    }

    Time restart()
    {
        return clock_->restart();
    }

private:

    Clock_ *clock_;
};
