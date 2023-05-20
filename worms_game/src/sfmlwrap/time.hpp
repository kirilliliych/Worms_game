#pragma once


#include <SFML/System.hpp>


class Time
{
    friend class Clock;

    class Time_ : public sf::Time
    {};

public:

    Time()
      : time_(new Time_())
    {}

    ~Time()
    {
        delete time_;
    }

    float as_seconds() const
    {
        return time_->asSeconds();
    }

private:

    Time(const sf::Time &sf_time)
      : Time()
    {
        *(reinterpret_cast<sf::Time *> (time_)) = sf_time;
    }

private:

    Time_ *time_;
};
