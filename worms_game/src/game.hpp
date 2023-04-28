#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics/CircleShape.hpp>
#include <algorithm>
#include <cassert>
#include <ctime>
#include <iostream>
#include <list>

#include "abstract_node.hpp"
#include "camera.hpp"
#include "desktop.hpp"
#include "map.hpp"
#include "sfmlwrap/surface.hpp"


namespace string_consts
{
    static const std::string GAME_TITLE = "Worms";
}


class WormsGame
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

    WormsGame(uint32_t window_width, uint32_t window_height,
              uint32_t map_width_,   uint32_t map_height, const std::string &title = string_consts::GAME_TITLE)
      : state_(GameState::UNINITIALIZED),
        main_window_(new Desktop(window_width, window_height, title)),
        map_(new Map(main_window_, map_width_, map_height)),
        camera_(new Camera(window_width, window_height))
    {}

    ~WormsGame()
    {
        delete main_window_;
        delete map_;
        delete camera_;
    }


    void run()
    {
        // state_ = GameState::CHOOSING_MODE;

        // state_ = GameState::GENERATING_TERRAIN;
        map_->create_map();
    
        main_window_->open();
        while (main_window_->is_open())
        {
            main_window_->redraw();
        }
    }

private:
//-----------------------------------Variables-------------------------------------
    GameState state_;

    Desktop *main_window_;
    Map *map_;
    Camera *camera_;
};

#endif
