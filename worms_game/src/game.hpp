#pragma once


#include <algorithm>
#include <cassert>
#include <chrono>
#include <ctime>
#include <iostream>

#include "abstract_node.hpp"
#include "fps.hpp"
#include "image_manager.hpp"
#include "physics_entity.hpp"
#include "sfmlwrap/clock.hpp"


namespace string_consts
{
    static const std::string GAME_TITLE = "Worms";
}


class Camera;
class Character;
class Desktop;
class EventManager;
class Map;
class PhysicsObject;
class Projectile;
class Team;
class AI;
class TurnTimeCounter;

class Game
{

public:

    Game(uint32_t window_width, uint32_t window_height,
         uint32_t map_width, uint32_t map_height,
         const std::string &title = string_consts::GAME_TITLE);
    
    ~Game();

    void run();

    void add_to_map_children(AbstractNode *object);

    bool launch_event(const Event &event);

    bool is_under_control(const AbstractNode *object) const;

    bool player_action_finished() const;

    void finish_player_action();

    void enable_player_action();

    bool does_player_have_control() const;

    void set_player_control(bool whether_has_control);

    const Character *get_character_under_control() const;

    const PhysicsObject *get_camera_tracking_object() const;

    uint32_t get_window_width() const;
    
    uint32_t get_window_height() const;

    uint32_t get_map_width() const;
    
    uint32_t get_map_height() const;

    Point2d<int> get_camera_position() const;

    bool is_stable() const;

    void lock_camera() const;

    void set_character_under_control(const Character *now_under_control);

    void set_camera_tracking_object(const PhysicsObject *now_tracked_object);

    Team *get_team(uint32_t index);

private:

    void pass_turn_();
//-----------------------------------Variables-------------------------------------
    using clock = std::chrono::system_clock;
    using time_point = std::chrono::time_point<clock>;
    using time_delta_t = std::chrono::duration<float, std::chrono::seconds::period>;

public:

    static constexpr uint32_t TEAMS_QUANTITY   = 3;
    static constexpr uint32_t MEMBERS_QUANTITY = 3; 

    static constexpr uint32_t EVENTS_HANDLING_PER_FRAME = 1;

    static Game *game;
    static ImageManager imanager;

    time_delta_t time_delta;
    time_point prev_time_point;

private:

    Desktop *main_window_;
    Map *map_;
    Camera *camera_;

    EventManager *emanager_;

    TurnTimeCounter *turn_timer_;
    FPSCounter *fps_counter_;

    vector<Team *> teams_;
    uint32_t active_team_index_;

    AI *ai_;

    const Character *under_control_;
    const PhysicsObject * camera_tracking_;

    bool player_has_control_;
    bool player_action_finished_;

    bool is_stable_;
};
