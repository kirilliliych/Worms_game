#pragma once


#include "abstract_node.hpp"
#include "game.hpp"
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
        move_speed_(DEFAULT_MOVE_SPEED)
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
                // printf("mmedata: %d %d\n", event.mmedata_.position.x(), event.mmedata_.position.y());
                // printf("area x: %d y: %d\n", area_.left_top().x(), area_.left_top().y());

                if (event.mmedata_.position.x() - area_.left_top().x() < DEFAULT_CAMERA_MOVE_MARGIN)
                {
                    area_.set_left_top_x(static_cast<float> (area_.left_top().x()) - move_speed_ * Game::time_delta.count());
                }
                if (event.mmedata_.position.x() - area_.left_top().x() > area_.get_width() - DEFAULT_CAMERA_MOVE_MARGIN)
                {
                    area_.set_left_top_x(static_cast<float> (area_.left_top().x()) + move_speed_ * Game::time_delta.count());
                }
                if (event.mmedata_.position.y() - area_.left_top().y() < DEFAULT_CAMERA_MOVE_MARGIN)
                {
                    area_.set_left_top_y(static_cast<float> (area_.left_top().y()) - move_speed_ * Game::time_delta.count());
                }
                if (event.mmedata_.position.y() - area_.left_top().y() > area_.get_height() - DEFAULT_CAMERA_MOVE_MARGIN)
                {
                    area_.set_left_top_y(static_cast<float> (area_.left_top().y()) + move_speed_ * Game::time_delta.count());
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

                break;
            }

            default:
            {
                result = children_handle_event(event);
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
};
