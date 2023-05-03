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

    Camera(AbstractNode *parent, const Rect<int> &area)
      : AbstractNode(parent, area),
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

    bool handle_event(const Event &event)
    {
        bool result = false;
        // if (event.get_type() == EventType::MOUSE_MOVED)
        // {
        //     printf("entered camera handle event mouse moved\n");
        //     const MouseMovedEvent &mme = reinterpret_cast<const MouseMovedEvent &> (event);
        //     printf("position x: %d y: %d\n", mme.position_.x(), mme.position_.y());
        // }
        switch (event.get_type())
        {
            case EventType::MOUSE_MOVED:
            {
                // const MouseMovedEvent &mme = reinterpret_cast<const MouseMovedEvent &> (event);
                if (event.mmedata_.position.x() < DEFAULT_CAMERA_MOVE_MARGIN)
                {
                    // printf("time: %lg\n", Game::time_delta.count());
                    area_.set_left_top_x(static_cast<float> (area_.left_top().x()) - move_speed_ * Game::time_delta.count());
                }
                if (event.mmedata_.position.x() > area_.get_width() - DEFAULT_CAMERA_MOVE_MARGIN)
                {
                    // printf("time: %lg\n", Game::time_delta.count());
                    area_.set_left_top_x(static_cast<float> (area_.left_top().x()) + move_speed_ * Game::time_delta.count());
                }
                if (event.mmedata_.position.y() < DEFAULT_CAMERA_MOVE_MARGIN)
                {
                    // printf("time: %lg\n", Game::time_delta.count());
                    area_.set_left_top_y(static_cast<float> (area_.left_top().y()) - move_speed_ * Game::time_delta.count());
                }
                if (event.mmedata_.position.y() > area_.get_height() - DEFAULT_CAMERA_MOVE_MARGIN)
                {
                    // printf("time: %lg\n", Game::time_delta.count());
                    area_.set_left_top_y(static_cast<float> (area_.left_top().y()) + move_speed_ * Game::time_delta.count());
                }
                
                if (area_.left_top().x() < 0)
                {
                    area_.set_left_top_x(0);
                }
                if (area_.left_top().x() > static_cast<int> (Game::map_width_ - Game::window_width_))
                {
                    area_.set_left_top_x(Game::map_width_ - Game::window_width_);
                }
                if (area_.left_top().y() < 0)
                {
                    area_.set_left_top_y(0);
                }
                if (area_.left_top().y() > static_cast<int> (Game::map_height_ - Game::window_height_))
                {
                    area_.set_left_top_y(Game::map_height_ - Game::window_height_);
                }

                break;
            }

            default:
            {
                for (uint32_t child_index = 0; child_index < children_.size(); ++child_index)
                {
                    result = children_[child_index]->handle_event(event);
                }
            }
        }

        return result;
    }

private:

    static constexpr float DEFAULT_MOVE_SPEED = 5000.f;
    static constexpr int DEFAULT_CAMERA_MOVE_MARGIN = 30;
    
    float move_speed_;
};
