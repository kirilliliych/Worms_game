#pragma once


#include <algorithm>
#include <cassert>
#include <chrono>
#include <ctime>
#include <iostream>
#include <list>

#include "abstract_node.hpp"
#include "image_manager.hpp"
#include "physics_entity.hpp"


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

class Game
{
    enum class GameState
    {
        UNINITIALIZED,
        CHOOSING_MODE,
        GENERATING_TERRAIN,
        GENERATING_UNITS,
        READY,
        PLAYER_CONTROL,
        CAMERA_CONTROL,
        EXITED
    };

public:

    Game(uint32_t window_width, uint32_t window_height,
         uint32_t map_width, uint32_t map_height,
         const std::string &title = string_consts::GAME_TITLE);
    
    ~Game();


    void run();

    void add_to_map_children(AbstractNode *object);

    // bool check_collision(const void *checker_address, PhysicsEntity checker, const Point2d<int> &collision_point) const;

    // void process_explosion(float radius, const Point2d<int> &position);

    bool launch_event(const Event &event);

    bool is_under_control(const AbstractNode *object) const;

    const Character *get_character_under_control() const;

    const PhysicsObject *get_camera_tracking_object() const;

    uint32_t get_window_width() const;
    
    uint32_t get_window_height() const;

    uint32_t get_map_width() const;
    
    uint32_t get_map_height() const;

    Point2d<int> get_camera_position() const;

    void lock_camera() const;

private:
//-----------------------------------Variables-------------------------------------
    using clock = std::chrono::system_clock;
    using time_point = std::chrono::time_point<clock>;
    using time_delta_t = std::chrono::duration<float, std::chrono::seconds::period>;
    static time_point prev_time_point;

public:

    static Game *game;

    static ImageManager imanager;
    static time_delta_t time_delta;

private:
public:
    GameState state_;

    Desktop *main_window_;
    Map *map_;
    Camera *camera_;

    EventManager *emanager_;

public:
    const Character *under_control_;
    const PhysicsObject * camera_tracking_; 
    Team *team_;   // list of teams?
};
