#include <chrono>
#include <cstdlib>
#include "character.hpp"
#include "desktop.hpp"
#include "game.hpp"
#include "image_manager.hpp"
#include "map.hpp"
#include "physics_object.hpp"
#include "projectile.hpp"
#include "sfmlwrap/events/time_event.hpp"
#include "team.hpp"


Game *Game::game = nullptr;


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
                                                    {0, 0}})),
    under_control_(nullptr),
    rocket_(new Projectile(map_, {40, 40, {500, 200}}, "rocket.png"))
{
    assert(game == nullptr);    // singleton
    game = this;

    window_width_  = window_width;
    window_height_ = window_height;
    map_width_     = map_width;
    map_height_    = map_height; 
}

Game::~Game()
{
    delete main_window_;
    delete map_;
    delete team_;

    delete rocket_;
}


void Game::run()
{
    // state_ = GameState::CHOOSING_MODE;

    // state_ = GameState::GENERATING_TERRAIN;
    map_->create_map();

    // state_ = GameState::GENERATING_UNITS;
    team_ = new Team(map_, 3, 400, 300, 40, 40);
    under_control_ = team_->get_next_character();
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
        main_window_->handle_event(time_event);

        main_window_->redraw();
    }
}

uint32_t Game::add_to_map_children(AbstractNode *object)
{
    assert(object != nullptr);

    return map_->add_child(object);
}

bool Game::check_collision(const void *checker_address, PhysicsEntity checker, const Point2d<int> &collision_point) const
{
    assert(checker_address != nullptr);

    Event collision_event;
    collision_event.set_type(EventType::COLLISION_EVENT);
    collision_event.cedata_.checker  = checker;
    collision_event.cedata_.position = collision_point;
    
    return main_window_->handle_event(collision_event);
}

void Game::process_explosion(float radius, const Point2d<int> &position)
{
    Event explosion_event;
    explosion_event.set_type(EventType::EXPLOSION_EVENT);
    explosion_event.eedata_.radius   = radius;
    explosion_event.eedata_.position = position;

    main_window_->handle_event(explosion_event); 
}

const Character *Game::get_character_under_control() const
{
    return under_control_;
}
