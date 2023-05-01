#include "game.hpp"
#include "image_manager.hpp"
#include "map.hpp"


ImageManager Game::imanager{};


Game::Game(uint32_t window_width, uint32_t window_height,
           uint32_t map_width,    uint32_t map_height,
           const std::string &title)
  : state_(GameState::UNINITIALIZED),
    main_window_(window_width, window_height, title),
    map_(new Map(&main_window_, map_width, map_height)),
    camera_(new Camera(window_width, window_height)),
    test_dude_(new Character(map_, {100, 100, {150, 200}},  // currently width and height does not make sense; they will be needed for scaling
                        "standing.png"))
{}

Game::~Game()
{
    delete map_;
    delete camera_;

    delete test_dude_;
}


void Game::run()
{
    // state_ = GameState::CHOOSING_MODE;

    // state_ = GameState::GENERATING_TERRAIN;
    map_->create_map();

    // state_ = GameState::GENERATING_UNITS;
    // here alloc team 

    main_window_.open();
    while (main_window_.is_open())
    {
        main_window_.redraw();
    }
}   
