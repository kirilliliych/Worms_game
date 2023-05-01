#include "abstract_node.hpp"
#include "game.hpp"
#include "sfmlwrap/texture.hpp"


    AbstractNode::AbstractNode(AbstractNode *parent, const Rect<int> &area)
      : parent_(parent),
        children_(),
        area_(area),
        texture_(new Texture()) // remove?
    {
        if (parent != nullptr)
        {
            parent->children_.push_back(this);
        }
    }  
    
    AbstractNode::AbstractNode(AbstractNode *parent, const Rect<int> &area, const std::string &image_file_name,
                               const Rect<int> &texture_area)
      : parent_(parent),
        children_(),
        area_(area),
        texture_(new Texture())
    {
        if (parent != nullptr)
        {
            parent->children_.push_back(this);
        }

        const Image *texture_image = Game::imanager.get_image(image_file_name);
        assert(texture_image != nullptr);
        texture_->load_from_image(*texture_image, texture_area);

        // rewrite to sprite; do scale
    }

    AbstractNode::~AbstractNode()
    {
        delete texture_;
    }

    void AbstractNode::render(Surface *surface)
    {
        assert(surface != nullptr);

        render_self(surface);
        render_children(surface);
    }

    void AbstractNode::render_self(Surface *surface)
    {
        assert(surface != nullptr);

        Sprite self_sprite(*texture_, area_.left_top());
        surface->draw_sprite(self_sprite);
    }

    void AbstractNode::render_children(Surface *surface)
    {
        assert(surface != nullptr);

        auto children_iterator = children_.begin();
        auto children_end = children_.end();
        while (children_iterator != children_end)
        {
            (*children_iterator)->render(surface);
            
            ++children_iterator;
        }
    }


