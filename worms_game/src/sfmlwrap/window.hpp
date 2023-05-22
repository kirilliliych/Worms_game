#ifndef WINDOW_HPP
#define WINDOW_HPP


#include <SFML/System/Vector2.hpp>
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

    uint32_t get_width() const
    {
        return width_;
    }

    uint32_t get_height() const
    {
        return height_;
    }

    bool poll_event(Event *event) const
    {
        sf::Event sf_event;
        if (window_->pollEvent(sf_event))
        {
            switch (sf_event.type)
            {
                case sf::Event::KeyPressed:
                {
                    event->set_type(EventType::KEY_PRESSED);
                    event->kedata_ = *reinterpret_cast<KeyEventData *> (&sf_event.key);

                    break;
                }

                case sf::Event::KeyReleased:
                {
                    event->set_type(EventType::KEY_RELEASED);
                    event->kedata_ = *reinterpret_cast<KeyEventData *> (&sf_event.key);

                    break;
                }

                case sf::Event::Closed:
                {
                    event->set_type(EventType::QUIT_EVENT);

                    break;
                }

                case sf::Event::MouseButtonPressed:
                {
                    event->set_type(EventType::MOUSE_BUTTON_PRESSED);
                    event->mbedata_.button = recognize_mouse_button_(sf_event);
                    event->mbedata_.position = {sf_event.mouseButton.x,
                                                sf_event.mouseButton.y
                                               };

                    break;
                }

                case sf::Event::MouseButtonReleased:
                {
                    event->set_type(EventType::MOUSE_BUTTON_RELEASED);
                    event->mbedata_.button = recognize_mouse_button_(sf_event);
                    event->mbedata_.position = {sf_event.mouseButton.x,
                                                sf_event.mouseButton.y
                                               };

                    break;
                }

                case sf::Event::MouseMoved:
                {
                    event->set_type(EventType::MOUSE_MOVED);
                    event->mmedata_.position = {sf_event.mouseMove.x,
                                                sf_event.mouseMove.y
                                               };

                    break;
                }

                default:
                {
                    event->set_type(EventType::OTHER_EVENT);

                    return false;
                }
            }

            return true;
        }

        return false;
    }

private:

    MouseButton recognize_mouse_button_(const sf::Event &event) const
    {
        switch (event.mouseButton.button)
        {
            case sf::Mouse::Left:
            {
                return MouseButton::LEFT;
            }

            case sf::Mouse::Right:
            {
                return MouseButton::RIGHT;
            }

            case sf::Mouse::Middle:
            {
                return MouseButton::WHEEL;
            }

            case sf::Mouse::XButton1:
            {
                return MouseButton::EXTRA1;
            }

            case sf::Mouse::XButton2:
            {
                return MouseButton::EXTRA2;
            }

            default:
            {
                return MouseButton::UNKNOWN;
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
