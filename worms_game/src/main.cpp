#include <SFML/Graphics/CircleShape.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "Events/Events.hpp"


int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    OtherEvent other{};

    TimeEvent dt_event{3};

    std::cout << dt_event.dt_ << '\n';

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                QuitEvent quit_event{};
                window.close();
            }
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}