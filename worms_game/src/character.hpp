#pragma once


#include "abstract_node.hpp"
#include "crosshair.hpp"
#include "debris.hpp"
#include "physics_object.hpp"
#include "sfmlwrap/events/event.hpp"
#include "sfmlwrap/texture.hpp"


enum class CharacterState
{
    PASSIVE,
    MOVING,
    ARMED,
    HIT,
    DEAD,
    UNKNOWN
};

class CharacterUI;
class Weapon;

class Character : public PhysicsObject
{
public:

    Character(AbstractNode *parent, const Rect<int> &area, int hp, uint32_t color);

    Character(AbstractNode *parent, const Rect<int> &area, int hp, const std::string &image_file_name, uint32_t color);

    ~Character();

    CharacterState get_state() const;

    bool is_alive() const;

    int get_hp() const;

    void set_hp(int new_hp);

    void remove_weapon();

    void on_bounce_death(const Point2d<int> &death_position) override;

    void kill() override;

    void render_self(Surface *surface, const Point2d<int> &camera_offset) override;

    bool handle_event(const Event &event) override;

private:

    void set_texture_(float OX_angle);

private:

    static constexpr float DEFAULT_FRICTION = 0.000002f;
    static constexpr float ANIMATION_IMAGE_CHANGE_MIN_DELAY = 2.f;

    CharacterState state_;
    std::string animation_cur_image_name_;
    float animation_image_change_cur_delay_;
    bool switch_to_passive_stance_;

    Crosshair *crosshair_;    
    Weapon *weapon_;

    int hp_;
    CharacterUI *character_ui_; 
};

