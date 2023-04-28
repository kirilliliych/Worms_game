#ifndef DESKTOP_HPP
#define DESKTOP_HPP


#include "abstract_node.hpp"
#include "sfmlwrap/rect.hpp"
#include "sfmlwrap/surface.hpp"
#include "sfmlwrap/texture.hpp"
#include "sfmlwrap/window.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>


class Desktop final : public Window, public AbstractNode
{
public:

    Desktop(uint32_t width, uint32_t height, const std::string &title)
      : Window(width, height, title),
        AbstractNode(nullptr, Rect<int>(static_cast<int> (width),
                                        static_cast<int> (height),
                                        {0, 0})),
        main_surface_(new Surface())
    {
        id = 0;
        main_surface_->create(width, height);
    }

    ~Desktop()
    {
        delete main_surface_;
    }

    void render_self(Surface *surface) override
    {
        assert(surface != nullptr);
    }

    void redraw()
    {
        clear();
        // sf::CircleShape shape(100.f);
        // shape.setFillColor(sf::Color::Green);
        main_surface_->clear();
        // window_->draw(shape);
        // main_surface_->surface_->clear(sf::Color::Blue);
        // main_surface_->surface_->draw(shape);
        // main_surface_->surface_->display();
        render(main_surface_);
        main_surface_->update();
        // window_->draw(sf::Sprite(main_surface_->surface_->getTexture()));
        draw_surface(main_surface_);

        update();

        // clear();
        // Texture tex;
        // assert(tex.load_from_file("textures/seamless_sky.png"));
        // texture_->load_from_file("textures/seamless_sky.png");
        // rt.clear();
        // rt.draw(sf::Sprite(*texture_->texture_));
        // rt.display();
        // window_->draw(sf::Sprite(rt.getTexture()));
        // main_surface_->clear();
        // main_surface_->draw_sprite(Sprite(texture_));
        // main_surface_->update();
        // draw_surface(main_surface_);
        // Sprite surface_sprite(main_surface_->get_texture());
        // window_->draw(*surface_sprite.sprite_);

        // window_->draw(sf::Sprite(main_surface_->surface_->getTexture()));
        // update();
    }

private:

    Surface *main_surface_;
};

#endif
