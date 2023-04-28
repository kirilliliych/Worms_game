#ifndef ABSTRACT_NODE_HPP
#define ABSTRACT_NODE_HPP


#include <cassert>
#include "point2d.hpp"
#include "sfmlwrap/surface.hpp"
#include "sfmlwrap/texture.hpp"
#include "sfmlwrap/rect.hpp"


class AbstractNode
{
public:

    AbstractNode(AbstractNode *parent, Rect<int> area = Rect<int>())
      : parent_(parent),
        children_(),
        area_(area),    // NOTE: might be zero area!
        texture_(new Texture())
    {
        if (parent != nullptr)
        {
            parent->children_.push_back(this);
        }
    }  
    
    AbstractNode(AbstractNode *parent, Rect<int> area, const std::string &texture_file_name,
                 Rect<int> texture_area = Rect<int>())
      : parent_(parent),
        children_(),
        area_(area),
        texture_(new Texture(texture_file_name, texture_area))
    {}

    virtual ~AbstractNode()
    {
        delete texture_;
    }

    void render(Surface *surface)
    {
        assert(surface != nullptr);

        render_self(surface);
        render_children(surface);
    }

    virtual void render_self(Surface *surface)
    {
        assert(surface != nullptr);

        Sprite self_sprite = Sprite(*texture_, area_.left_top());
        surface->draw_sprite(self_sprite);
    }

    void render_children(Surface *surface)
    {
        assert(surface != nullptr);
        // auto children_iterator = children_.begin();
        // auto children_end = children_.end();
        // while (children_iterator != children_end)
        // {
        //     (*children_iterator)->render(surface);
        //     ++children_iterator;
        // }

        for (uint64_t child_index = 0; child_index < children_.size(); ++child_index)
        {
            children_[child_index]->render(surface);
        }
    }

protected:

    AbstractNode *parent_;
    std::vector<AbstractNode *> children_;

    Rect<int> area_;

    Texture *texture_;

public:

    int id;
};

#endif
