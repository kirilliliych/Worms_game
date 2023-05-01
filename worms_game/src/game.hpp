#pragma once


#include <algorithm>
#include <cassert>
#include <ctime>
#include <iostream>
#include <list>

#include "abstract_node.hpp"
#include "camera.hpp"
#include "desktop.hpp"
#include "image_manager.hpp"
#include "team.hpp"
#include "sfmlwrap/surface.hpp"


namespace string_consts
{
    static const std::string GAME_TITLE = "Worms";
}


class Map;

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

private:
//-----------------------------------Variables-------------------------------------
    GameState state_;

public:
    static ImageManager imanager;
private:

    Desktop main_window_;
    Map *map_;
    Camera *camera_;

    Team *test_team_;
};
