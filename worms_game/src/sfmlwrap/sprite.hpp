#ifndef SPRITE_HPP
#define SPRITE_HPP


#include <SFML/Graphics.hpp>
#include "../point2d.hpp"
#include "texture.hpp"


class Sprite
{
    friend class Window;
    friend class Surface;

    class Sprite_ : public sf::Sprite
    {
    public:

        Sprite_()
          : sf::Sprite()
        {}

        Sprite_(const Texture &texture)
          : sf::Sprite(*texture.texture_)
        {}
    };

public:

    Sprite()
      : sprite_(new Sprite_())
    {}

    Sprite(const Texture &texture, Point2d<int> position = Point2d<int>())
      : sprite_(new Sprite_(texture))
    {
        set_position(position);
    }

    ~Sprite()
    {
        delete sprite_;
    }
    
    void set_texture(const Texture &texture)
    {
        sprite_->setTexture(*texture.texture_);
    }

    void set_position(Point2d<int> position)
    {
        sprite_->setPosition(sf::Vector2f(position.x(), position.y()));
    }

    void set_scale(float factor_x, float factor_y)
    {
        sprite_->setScale(factor_x, factor_y);
    }

    void set_rotation(float angle)
    {
        sprite_->setRotation(angle);
    }

    void rotate(float angle)
    {
        sprite_->rotate(angle);
    }

private:
public:
    Sprite_ *sprite_;
};

#endif
