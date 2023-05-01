#ifndef WINDOW_HPP
#define WINDOW_HPP


#include <SFML/Window/Event.hpp>
#include <string>
#include <SFML/Graphics.hpp>
#include "events/event.hpp"
#include "sfmlwrap/events/keyboard_events.hpp"
#include "sfmlwrap/events/mouse_events.hpp"
#include "sfmlwrap/events/quit_event.hpp"
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


protected:

    BaseEvent poll_event(bool *event_polled) const
    {
        sf::Event sf_event;
        if (window_->pollEvent(sf_event))
        {
            *event_polled = true;

            switch (sf_event.type)
            {
                case sf::Event::KeyPressed:
                {
                    return KeyPressedEvent(*(reinterpret_cast<KeyboardKey *> (&sf_event.key)));
                }

                case sf::Event::KeyReleased:
                {
                    return KeyReleasedEvent(*(reinterpret_cast<KeyboardKey *> (&sf_event.key)));
                }

                case sf::Event::Closed:
                {
                    return QuitEvent();
                }

                case sf::Event::MouseButtonPressed:
                {
                    return MouseClickedEvent(recognize_mouse_button_(sf_event), {sf_event.mouseButton.x,
                                                                                  sf_event.mouseButton.y});
                }

                case sf::Event::MouseButtonReleased:
                {
                    return MouseReleasedEvent(recognize_mouse_button_(sf_event), {sf_event.mouseButton.x,
                                                                                         sf_event.mouseButton.y});
                }

                case sf::Event::MouseMoved:
                {
                    return MouseMovedEvent({sf_event.mouseMove.x,
                                             sf_event.mouseMove.y});
                }

                default:
                {
                    return OtherEvent();
                }
            }
        }

        *event_polled = false;
    }

private:

    MouseButton recognize_mouse_button_(const sf::Event &event) const
    {
        switch (event.mouseButton.button)
        {
            case sf::Mouse::Left:
            {
                return MouseButton::Left;
            }

            case sf::Mouse::Right:
            {
                return MouseButton::Right;
            }

            case sf::Mouse::Middle:
            {
                return MouseButton::Wheel;
            }

            case sf::Mouse::XButton1:
            {
                return MouseButton::Extra1;
            }

            case sf::Mouse::XButton2:
            {
                return MouseButton::Extra2;
            }

            default:
            {
                return MouseButton::Unknown;
            }
        }
    }

private:
public:
    uint32_t width_;
    uint32_t height_;
    std::string title_;

    Window_ *window_;
};


#endif
