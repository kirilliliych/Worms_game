#ifndef SURFACE_HPP
#define SURFACE_HPP


#include <SFML/Graphics.hpp>
#include "../point2d.hpp"
#include "sprite.hpp"
#include "text.hpp"
#include "texture.hpp"


class Surface
{
    class Surface_ : public sf::RenderTexture
    {};

public:

    Surface()
      : surface_(new Surface_()),
        surface_texture_(new Texture())
    {}

    ~Surface()
    {
        delete surface_;
        delete surface_texture_;
    }

    bool create(uint32_t width, uint32_t height)
    {
        return surface_->create(width, height);
    }

    const Texture &get_texture() const
    {
        return *surface_texture_;
    }

    void draw_sprite(const Sprite &sprite)
    {
        surface_->draw(*sprite.sprite_);

        update();
    }

    void draw_text(const Text &text)
    {
        surface_->draw(*text.text_);
        update();
    }

    void clear()
    {
        surface_->clear();
    }

    void update()
    {
        surface_->display();
        *(reinterpret_cast<sf::Texture *> (surface_texture_->texture_)) = surface_->getTexture();
    }

private:
public:
    Surface_ *surface_;
    Texture *surface_texture_;
};


#endif
