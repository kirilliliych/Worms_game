#pragma once


#include "abstract_node.hpp"
#include "game.hpp"
#include "physics_object.hpp"
#include "sfmlwrap/events/mouse_events.hpp"
#include "sfmlwrap/events/event.hpp"
#include "point2d.hpp"
#include "sfmlwrap/events/base_event.hpp"


class Camera : public AbstractNode
{
public:

    Camera(AbstractNode *parent, const Rect<int> &area, uint32_t max_x, uint32_t max_y)
      : AbstractNode(parent, area),
        max_x_(max_x),
        max_y_(max_y),
        move_speed_(DEFAULT_MOVE_SPEED),
        is_locked_(false)
    {}
    
    int get_x() const
    {
        return area_.left_top().x();
    }

    int get_y() const
    {
        return area_.left_top().y();
    }

    Point2d<int> get_position() const
    {
        return area_.left_top();
    }

    bool is_locked() const
    {
        return is_locked_;
    }

    void set_x(int x)
    {
        area_.set_left_top_x(x);
    }

    void set_y(int y)
    {
        area_.set_left_top_y(y);
    }

    void set_position(Point2d<int> position)
    {
        area_.set_left_top(position);
    }

    void lock()
    {
        is_locked_ = true;
    }

    void unlock()
    {
        is_locked_ = false;
    }

    void set_move_speed(float camera_move_speed)
    {
        move_speed_ = camera_move_speed;
    }

    void render_self(Surface *surface, const Point2d<int> &camera_offset) override
    {
        assert(surface != nullptr);
    }

    bool handle_event(const Event &event) override
    {
        bool result = false;

        switch (event.get_type())
        {
            case EventType::MOUSE_MOVED:
            {
                if (event.mmedata_.position.x() - area_.left_top().x() < DEFAULT_CAMERA_MOVE_MARGIN)
                {
                    is_locked_ = false;
                    area_.set_left_top_x(static_cast<float> (area_.left_top().x()) - move_speed_ * Game::time_delta.count());
                }
                if (event.mmedata_.position.x() - area_.left_top().x() > area_.get_width() - DEFAULT_CAMERA_MOVE_MARGIN)
                {
                    is_locked_ = false;
                    area_.set_left_top_x(static_cast<float> (area_.left_top().x()) + move_speed_ * Game::time_delta.count());
                }
                if (event.mmedata_.position.y() - area_.left_top().y() < DEFAULT_CAMERA_MOVE_MARGIN)
                {
                    is_locked_ = false;
                    area_.set_left_top_y(static_cast<float> (area_.left_top().y()) - move_speed_ * Game::time_delta.count());
                }
                if (event.mmedata_.position.y() - area_.left_top().y() > area_.get_height() - DEFAULT_CAMERA_MOVE_MARGIN)
                {
                    is_locked_ = false;
                    area_.set_left_top_y(static_cast<float> (area_.left_top().y()) + move_speed_ * Game::time_delta.count());
                }

                for (uint32_t child_index = 0; child_index < children_.size(); ++child_index)
                {
                    if (children_[child_index]->handle_event(event))
                    {
                        result = true;
                    }
                }

                break;
            }
            case EventType::TIME_PASSED:
            {
                const PhysicsObject *camera_tracking = Game::game->get_camera_tracking_object();
                if ((camera_tracking != nullptr) && is_locked_)
                {
                    set_position(camera_tracking->get_area().left_top() - area_.half_size());
                }
                
                if (area_.left_top().x() < 0)
                {
                    area_.set_left_top_x(0);
                }
                if (area_.left_top().x() > static_cast<int> (max_x_))
                {
                    area_.set_left_top_x(static_cast<int> (max_x_));
                }
                if (area_.left_top().y() < 0)
                {
                    area_.set_left_top_y(0);
                }
                if (area_.left_top().y() > static_cast<int> (max_y_))
                {
                    area_.set_left_top_y(static_cast<int> (max_y_));
                }

                for (uint32_t child_index = 0; child_index < children_.size(); ++child_index)
                {
                    if (children_[child_index]->handle_event(event))
                    {
                        result = true;
                    }
                }

                break;
            }

            default:
            {
                // result = children_handle_event(event);

                for (uint32_t child_index = 0; child_index < children_.size(); ++child_index)
                {
                    if (children_[child_index]->handle_event(event))
                    {
                        result = true;
                    }
                }
            }
        }

        return result;
    }

private:

    static constexpr float DEFAULT_MOVE_SPEED = 5000.f;
    static constexpr int DEFAULT_CAMERA_MOVE_MARGIN = 30;
    
    uint32_t max_x_;
    uint32_t max_y_;

    float move_speed_;

    bool is_locked_;
};
