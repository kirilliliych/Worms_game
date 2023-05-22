#pragma once

#include <cmath>
#include "abstract_node.hpp"
#include "event_manager.hpp"
#include "game.hpp"
#include "physics_entity.hpp"
#include "sfmlwrap/events/event.hpp"
#include "sfmlwrap/rect.hpp"
#include "vector2d.hpp"


class PhysicsObject : public AbstractNode
{
protected:

    PhysicsObject(AbstractNode *parent, const Rect<int> &area, const Vector2d<float> &velocity,
                  const Vector2d<float> &acceleration, float friction, int bounces_before_death);

    PhysicsObject(AbstractNode *parent, const Rect<int> &area, const Vector2d<float> &velocity,
                  const Vector2d<float> &acceleration, float friction, int bounces_before_death,
                  const std::string &texture_file_name);
                  
    virtual void on_bounce_death(const Point2d<int> &death_position) = 0;
    
public:

    PhysicsEntity get_physics_entity_type() const;

    bool is_stable() const;

    void handle_physics();

    bool handle_event(const Event &event) override;

private:

    template<typename Type>
    bool point_is_in_correct_semicircle_zone_(float angle_coef, bool is_above_semicircle_flat_part, const Point2d<Type> &point) const
    {
        return (( is_above_semicircle_flat_part && (point.y() <= angle_coef * point.x())) ||
                (!is_above_semicircle_flat_part && (point.y() >  angle_coef * point.x())));
    }

protected:

    PhysicsEntity type_;

    Vector2d<float> velocity_;
    Vector2d<float> acceleration_;
    float friction_;

    int bounces_before_death_;

    bool is_stable_;

    float radius_;
};
