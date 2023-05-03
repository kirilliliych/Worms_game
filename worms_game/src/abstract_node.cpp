#include "abstract_node.hpp"
#include "game.hpp"


AbstractNode::AbstractNode(AbstractNode *parent, const Rect<int> &area)
  : parent_(parent),
    children_(),
    area_(area),
    texture_(new Texture()),
    exists_(true)
{
    if (parent != nullptr)
    {
        parent->children_.push_back(std::unique_ptr<AbstractNode> (this));
    }
}  
    
AbstractNode::AbstractNode(AbstractNode *parent, const Rect<int> &area, const std::string &image_file_name,
                           const Rect<int> &texture_area)
  : parent_(parent),
    children_(),
    area_(area),
    texture_(new Texture()),
    exists_(true)
{
    if (parent != nullptr)
    {
        parent->children_.push_back(std::unique_ptr<AbstractNode> (this));
    }

    const Image *texture_image = Game::imanager.get_image(image_file_name);
    assert(texture_image != nullptr);
    texture_->load_from_image(*texture_image, texture_area);
}

AbstractNode::~AbstractNode()
{
    delete texture_;
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

uint32_t AbstractNode::add_child(AbstractNode *child)
{
    assert(child != nullptr);

    children_.push_back(std::unique_ptr<AbstractNode> (child));
    child->parent_ = this;

    return children_.size() - 1;
}

bool AbstractNode::does_exist() const
{
    return exists_;
}

bool AbstractNode::handle_event(const Event &event)
{
    bool result = false;

    switch (event.get_type())
    {
        default:
        {
            for (uint32_t child_index = 0; child_index < children_.size(); ++child_index)
            {
                if (children_[child_index]->handle_event(event))
                {
                    result = true;
                }
            }
            // result = children_handle_event(event);
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
