#pragma once


#include <cassert>
#include <memory>
#include "point2d.hpp"
#include "sfmlwrap/surface.hpp"
#include "sfmlwrap/texture.hpp"
#include "sfmlwrap/rect.hpp"
#include "stl.hpp"


class Event;

class AbstractNode
{
public:

    AbstractNode(AbstractNode *parent, const Rect<int> &area);
    
    AbstractNode(AbstractNode *parent, const Rect<int> &area, const std::string &image_file_name);

    virtual ~AbstractNode();


    const Rect<int> &get_area() const;

    void calculate_scale();

    void render(Surface *surface, const Point2d<int> &camera_offset);

    virtual void render_self(Surface *surface, const Point2d<int> &camera_offset);

    void render_children(Surface *surface, const Point2d<int> &camera_offset);

    void add_child(AbstractNode *child);

    bool does_exist() const;

    virtual void kill();

    virtual bool handle_event(const Event &event);

    bool children_handle_event(const Event &event);

protected:

    bool load_texture_from_image_manager(const std::string &image_file_name);

    AbstractNode *parent_;
    vector<AbstractNode *> children_;

    Rect<int> area_;

    Texture *texture_;
    float texture_scale_;

    bool exists_;
};
