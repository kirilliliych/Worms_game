#ifndef WINDOW_HPP
#define WINDOW_HPP


#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Window.hpp>
#include <string>
#include <SFML/Graphics.hpp>

#include "event.hpp"
#include "surface.hpp"


class Window
{
    class Window_ : public sf::RenderWindow
    {};

public:

    Window(uint32_t width, uint32_t height, const std::string &title)
      : width_(width),
        height_(height),
        title_(title),
        window_(new Window_())
    {}

    ~Window()
    {
        delete window_;
    }


    void create(uint32_t width, uint32_t height, const std::string &title)
    {
        window_->create(sf::VideoMode(width, height), title);
    }

    void open()
    {
        create(width_, height_, title_);
    }

    bool is_open() const
    {
        return window_->isOpen();
    }

    void close()
    {
        window_->close();
    }

    void clear()
    {
        window_->clear();
    }

    void update()
    {
        window_->display();
    }

    void draw_surface(const Surface *surface)
    {
        assert(surface != nullptr);

        Sprite surface_sprite(surface->get_texture());
        window_->draw(*surface_sprite.sprite_);
    }


    bool poll_event(Event &event)
    {
        // transformation of sf::Event to event

        sf::Event ev;
        if (window_->pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
            {
                window_->close();
            }

            return true;
        }

        return false;
    }

private:
public:
    uint32_t width_;
    uint32_t height_;
    std::string title_;

    Window_ *window_;
};


#endif
