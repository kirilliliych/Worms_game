#include "abstract_node.hpp"
#include "sfmlwrap/events/event.hpp"
#include "game.hpp"


AbstractNode::AbstractNode(AbstractNode *parent, const Rect<int> &area)
  : parent_(parent),
    children_(),
    area_(area),
    texture_(new Texture()),
    texture_scale_(1),
    exists_(true)
{
    if (parent != nullptr)
    {
        // parent->children_.push_back(std::unique_ptr<AbstractNode> (this));
        parent->children_.push_back(this);
    }

    uint32_t width  = static_cast<uint32_t> (area.width());
    uint32_t height = static_cast<uint32_t> (area.height()); 
    if ((width  != 0) &&
        (height != 0))
    {
        texture_->create(width, height);
    }
}  
    
AbstractNode::AbstractNode(AbstractNode *parent, const Rect<int> &area, const std::string &image_file_name)
  : parent_(parent),
    children_(),
    area_(area),
    texture_(new Texture()),
    texture_scale_(1),
    exists_(true)
{
    if (parent != nullptr)
    {
        // parent->children_.push_back(std::unique_ptr<AbstractNode> (this));
        parent->children_.push_back(this);
    }

    load_texture_from_image_manager(image_file_name);
}

AbstractNode::~AbstractNode()
{
    delete texture_;
}

const Rect<int> &AbstractNode::get_area() const
{
    return area_;
}

void AbstractNode::calculate_scale()
{
    uint32_t texture_width  = texture_->get_width();
    uint32_t texture_height = texture_->get_height();
    float asked_width  = static_cast<float> (area_.width());
    float asked_height = static_cast<float> (area_.height());
    float x_scale = asked_width  / static_cast<float> (texture_width);
    float y_scale = asked_height / static_cast<float> (texture_height);
    if (y_scale < x_scale)
    {
        area_.set_width(y_scale * static_cast<int> (texture_width));
        texture_scale_ = y_scale;
    }
    else
    {
        area_.set_height(x_scale * static_cast<int> (texture_height));
        texture_scale_ = x_scale;
    }
}

void AbstractNode::render(Surface *surface, const Point2d<int> &camera_offset)
{
    assert(surface != nullptr);

    render_self(surface, camera_offset);
    render_children(surface, camera_offset);
}

void AbstractNode::render_self(Surface *surface, const Point2d<int> &camera_offset)
{
    assert(surface != nullptr);

    Sprite self_sprite(*texture_, area_.left_top() - camera_offset);
    surface->draw_sprite(self_sprite);
}

void AbstractNode::render_children(Surface *surface, const Point2d<int> &camera_offset)
{
    assert(surface != nullptr);
    
    auto children_iterator = children_.begin();
    auto children_end = children_.end();
    while (children_iterator != children_end)
    {
        (*children_iterator)->render(surface, camera_offset);
        
        ++children_iterator;
    }
}

void AbstractNode::add_child(AbstractNode *child)
{
    assert(child != nullptr);

    // children_.push_back(std::unique_ptr<AbstractNode> (child));
    children_.push_back(child);
    child->parent_ = this;
}

bool AbstractNode::does_exist() const
{
    return exists_;
}

void AbstractNode::kill()
{
    exists_ = false;
}

bool AbstractNode::handle_event(const Event &event)
{
    bool result = false;

    switch (event.get_type())
    {
        default:
        {
            if (children_handle_event(event))
            {
                result = true;
            }
        }
    };

    return result;
}

bool AbstractNode::children_handle_event(const Event &event)
{
    bool result = false;

    for (uint32_t child_index = 0; child_index < children_.size(); ++child_index)
    {
        if (children_[child_index]->handle_event(event))
        {
            result = true;
        }
    }

    return result;
}

bool AbstractNode::load_texture_from_image_manager(const std::string &image_file_name)
{
    const Image *texture_image = Game::game->imanager.get_image(image_file_name);
    assert(texture_image != nullptr);

    return texture_->load_from_image(*texture_image);
}
