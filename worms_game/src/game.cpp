#include <chrono>
#include <cstdlib>
#include "camera.hpp"
#include "character.hpp"
#include "desktop.hpp"
#include "event_manager.hpp"
#include "game.hpp"
#include "image_manager.hpp"
#include "map.hpp"
#include "physics_object.hpp"
#include "projectile.hpp"
#include "sfmlwrap/events/event.hpp"
#include "sfmlwrap/events/time_event.hpp"
#include "team.hpp"


Game *Game::game = nullptr;


ImageManager Game::imanager{};
Game::time_point Game::prev_time_point{};
Game::time_delta_t Game::time_delta{};


Game::Game(uint32_t window_width, uint32_t window_height,
           uint32_t map_width,    uint32_t map_height,
           const std::string &title)
  : state_(GameState::UNINITIALIZED),
    main_window_(new Desktop(window_width, window_height, title)),
    map_(new Map(main_window_, {static_cast<int> (map_width),
                                            static_cast<int> (map_height),
                                                    {0, 0}})),
    camera_(new Camera(map_, {static_cast<int> (window_width),
                                  static_cast<int> (window_height), {0, 0}},
                       map_width  - window_width,
                       map_height - window_height)),
    emanager_(new EventManager(main_window_)),
    under_control_(nullptr),
    camera_tracking_(nullptr)
{
    assert(game == nullptr);    // singleton
    game = this;
}

Game::~Game()
{
    delete main_window_;
    delete map_;
    delete camera_;
    delete emanager_;

    delete team_;
}


void Game::run()
{
    // state_ = GameState::CHOOSING_MODE;

    // state_ = GameState::GENERATING_TERRAIN;
    map_->create_map();

    // state_ = GameState::GENERATING_UNITS;
    team_ = new Team(map_, 3, 700, 300, 40, 40);
    under_control_ = team_->get_next_character();
    camera_tracking_ = under_control_;
    // state_ = GameState::READY;
    
    clock clock{};
    prev_time_point = clock.now();

    main_window_->open();
    while (main_window_->is_open())
    {
        time_point cur_time_point = clock.now();
        Game::time_delta = cur_time_point - prev_time_point;
        prev_time_point = cur_time_point;

        // main_window_->process_events();
        for (int i = 0; i < 2000; ++i)
        emanager_->process_external_events(main_window_);

        Event time_event;
        time_event.set_type(EventType::TIME_PASSED);
        time_event.dt_ = Game::time_delta;
        // main_window_->handle_event(time_event);
        emanager_->handle_event(time_event);

        main_window_->redraw(camera_->get_position());
    }
}

void Game::add_to_map_children(AbstractNode *object)
{
    assert(object != nullptr);

    map_->add_child(object);
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

bool Game::launch_event(const Event &event)
{
    return emanager_->handle_event(event);
}

bool Game::is_under_control(const AbstractNode *object) const
{
    return under_control_ == object;
}

const Character *Game::get_character_under_control() const
{
    return under_control_;
}

const PhysicsObject *Game::get_camera_tracking_object() const
{
    return camera_tracking_;
}

uint32_t Game::get_window_width() const
{
    return main_window_->get_width();
}
    
uint32_t Game::get_window_height() const
{
    return main_window_->get_height();
}

uint32_t Game::get_map_width() const
{
    return map_->get_width();
}

uint32_t Game::get_map_height() const
{
    return map_->get_height();
}

Point2d<int> Game::get_camera_position() const
{
    return camera_->get_position();
}
