#pragma once


#include <cassert>
#include "point2d.hpp"
#include "sfmlwrap/events/event.hpp"
#include "sfmlwrap/surface.hpp"
#include "sfmlwrap/texture.hpp"
#include "sfmlwrap/rect.hpp"


class AbstractNode
{
protected:

    AbstractNode(AbstractNode *parent, const Rect<int> &area);
    
    AbstractNode(AbstractNode *parent, const Rect<int> &area, const std::string &image_file_name,
                 const Rect<int> &texture_area = Rect<int>());

    virtual ~AbstractNode();


    void render(Surface *surface, const Point2d<int> &camera_offset);

    virtual void render_self(Surface *surface, const Point2d<int> &camera_offset);

    void render_children(Surface *surface, const Point2d<int> &camera_offset);

public:

    virtual bool handle_event(const Event &event);

protected:

    AbstractNode *parent_;
    std::vector<AbstractNode *> children_;

    Rect<int> area_;

    Texture *texture_;

public:

    int id;     // for debug
};
