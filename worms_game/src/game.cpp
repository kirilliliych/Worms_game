#include <chrono>
#include "character.hpp"
#include "desktop.hpp"
#include "game.hpp"
#include "image_manager.hpp"
#include "map.hpp"
#include "sfmlwrap/events/time_event.hpp"
#include "team.hpp"


ImageManager Game::imanager{};
Game::time_point Game::prev_time_point{};
Game::time_delta_t Game::time_delta{};
uint32_t Game::window_width_  = 0;
uint32_t Game::window_height_ = 0;
uint32_t Game::map_width_     = 0;
uint32_t Game::map_height_    = 0;


Game::Game(uint32_t window_width, uint32_t window_height,
           uint32_t map_width,    uint32_t map_height,
           const std::string &title)
  : state_(GameState::UNINITIALIZED),
    main_window_(new Desktop(window_width, window_height, title)),
    map_(new Map(main_window_, {static_cast<int> (map_width),
                                            static_cast<int> (map_height),
                                                    {0, 0}}))
{
    window_width_  = window_width;
    window_height_ = window_height;
    map_width_     = map_width;
    map_height_    = map_height; 
}

Game::~Game()
{
    delete map_;

    delete test_team_;
}


void Game::run()
{
    // state_ = GameState::CHOOSING_MODE;

    // state_ = GameState::GENERATING_TERRAIN;
    map_->create_map();

    // state_ = GameState::GENERATING_UNITS;
    test_team_ = new Team(map_, 3, 400, 300, 40, 40);

    // state_ = GameState::READY;
    
    clock clock{};
    prev_time_point = clock.now();

    main_window_->open();
    while (main_window_->is_open())
    {
        time_point cur_time_point = clock.now();
        Game::time_delta = cur_time_point - prev_time_point;
        prev_time_point = cur_time_point;

        main_window_->process_events();

        Event time_event;
        time_event.set_type(EventType::TIME_PASSED);
        time_event.dt_ = Game::time_delta;
        // main_window_.handle_event(time_event);

        main_window_->redraw();
    }
}   
