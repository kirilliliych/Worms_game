#pragma once


#include <cassert>
#include <memory>
#include "point2d.hpp"
#include "sfmlwrap/events/event.hpp"
#include "sfmlwrap/surface.hpp"
#include "sfmlwrap/texture.hpp"
#include "sfmlwrap/rect.hpp"


class AbstractNode
{
public:

    AbstractNode(AbstractNode *parent, const Rect<int> &area);
    
    AbstractNode(AbstractNode *parent, const Rect<int> &area, const std::string &image_file_name,
                 const Rect<int> &texture_area = Rect<int>());

    virtual ~AbstractNode();


    const Rect<int> &get_area() const;

    void calculate_scale();

    void render(Surface *surface, const Point2d<int> &camera_offset);

    virtual void render_self(Surface *surface, const Point2d<int> &camera_offset);

    void render_children(Surface *surface, const Point2d<int> &camera_offset);

    void add_child(AbstractNode *child);

    bool does_exist() const;

    virtual bool handle_event(const Event &event);

    bool children_handle_event(const Event &event);

protected:

    bool load_texture_from_image_manager(const std::string &image_file_name);

    AbstractNode *parent_;
    std::vector<std::unique_ptr<AbstractNode>> children_;       // on_delete event instead

    Rect<int> area_;

    Texture *texture_;
    float texture_scale_;

    bool exists_;
};
