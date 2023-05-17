#pragma once


#include "abstract_node.hpp"
#include "crosshair.hpp"
#include "debris.hpp"
#include "physics_object.hpp"
#include "sfmlwrap/events/event.hpp"
#include "sfmlwrap/texture.hpp"


class CharacterUI;
class Weapon;

class Character : public PhysicsObject
{
public:

    Character(AbstractNode *parent, const Rect<int> &area, uint32_t color);

    Character(AbstractNode *parent, const Rect<int> &area, const std::string &texture_file_name, uint32_t color);

    ~Character();

    int get_hp() const;

    void set_hp(int new_hp);

    void on_bounce_death(const Point2d<int> &death_position) override;

    void render_self(Surface *surface, const Point2d<int> &camera_offset) override;

    bool handle_event(const Event &event) override;

private:

    void set_texture_by_angle_(float OX_angle);

private:
public:

    static constexpr float DEFAULT_FRICTION = 0.000002;

    Crosshair *crosshair_;    
    Weapon *weapon_;

    int hp_;
    CharacterUI *character_ui_; 
};

