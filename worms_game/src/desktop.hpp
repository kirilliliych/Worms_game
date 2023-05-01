#pragma once


#include "abstract_node.hpp"
#include "sfmlwrap/events/base_event.hpp"
#include "sfmlwrap/rect.hpp"
#include "sfmlwrap/surface.hpp"
#include "sfmlwrap/texture.hpp"
#include "sfmlwrap/window.hpp"
#include "SFML/Graphics.hpp"


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

        main_surface_->clear();
        render(main_surface_);
        main_surface_->update();
        
        draw_surface(main_surface_);

        update();
    }

    void process_events() const;

private:

    Surface *main_surface_;
};

