#pragma once


#include <algorithm>
#include <cassert>
#include <chrono>
#include <ctime>
#include <iostream>
#include <list>

#include "abstract_node.hpp"
#include "collidable.hpp"
#include "image_manager.hpp"
#include "physics_entity.hpp"


namespace string_consts
{
    static const std::string GAME_TITLE = "Worms";
}


class Map;
class Desktop;
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

    uint32_t add_to_map_children(AbstractNode *object);

    bool check_collision(const void *checker_address, PhysicsEntity checker, const Point2d<int> &collision_point) const;

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

    static uint32_t window_width_;
    static uint32_t window_height_;
    static uint32_t map_width_;
    static uint32_t map_height_;
    
private:

    GameState state_;

    Desktop *main_window_;
    Map *map_;

    Team *test_team_;   // list of teams?
};
