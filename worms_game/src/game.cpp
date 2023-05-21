#include <chrono>
#include <cstdlib>
#include "camera.hpp"
#include "character.hpp"
#include "colors.hpp"
#include "desktop.hpp"
#include "event_manager.hpp"
#include "fps.hpp"
#include "game.hpp"
#include "image_manager.hpp"
#include "map.hpp"
#include "physics_object.hpp"
#include "projectile.hpp"
#include "sfmlwrap/events/event.hpp"
#include "sfmlwrap/events/time_event.hpp"
#include "team.hpp"
#include "turn_time_counter.hpp"


Game *Game::game = nullptr;
ImageManager Game::imanager{};

Game::Game(uint32_t window_width, uint32_t window_height,
           uint32_t map_width,    uint32_t map_height,
           const std::string &title)
  : main_window_(new Desktop(window_width, window_height, title)),
    map_(new Map(main_window_, {static_cast<int> (map_width),
                                            static_cast<int> (map_height),
                                                    {0, 0}})),
    camera_(new Camera(map_, {static_cast<int> (window_width),
                                  static_cast<int> (window_height), {0, 0}},
                       map_width  - window_width,
                       map_height - window_height)),
    emanager_(new EventManager(main_window_)),
    turn_timer_(new TurnTimeCounter(map_, {100, 100, {10, 120}})),
    fps_counter_(new FPSCounter(map_, {100, 100, {0, 0}})),
    teams_(TEAMS_QUANTITY),
    active_team_index_(0),
    under_control_(nullptr),
    camera_tracking_(nullptr),
    player_has_control_(true),
    player_action_finished_(false),
    is_stable_(false)
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
    delete turn_timer_;
    delete fps_counter_;

    for (uint32_t i = 0; i < TEAMS_QUANTITY; ++i)
    {
        delete teams_[i];
    }
}


void Game::run()
{
// creating map
    map_->create_map();
    
// init teams
    int window_width = static_cast<int> (get_window_width());

    int team_UI_start_pos_x = 50;
    int team_UI_start_pos_y = 50;
    int team_UI_y_step = 25;
    teams_[0] = new Team(map_,
                         3,
                         300,
                         200,
                         30,
                         40,
                         colors::RED,
                         {static_cast<int> (window_width * 0.9f), 10, {team_UI_start_pos_x, team_UI_start_pos_y}});
    team_UI_start_pos_y += team_UI_y_step;
    teams_[1] = new Team(map_,
                         3,
                         700,
                         200,
                         30,
                         40,
                         colors::GREEN,
                         {static_cast<int> (window_width * 0.9f), 10, {team_UI_start_pos_x, team_UI_start_pos_y}});
    team_UI_start_pos_y += team_UI_y_step;
    teams_[2] = new Team(map_,
                         3,
                         1300,
                         200,
                         30,
                         40,
                         colors::YELLOW,
                         {static_cast<int> (window_width * 0.9f), 10, {team_UI_start_pos_x, team_UI_start_pos_y}});
    under_control_ = teams_[0]->get_next_character();
    set_camera_tracking_object(under_control_);
    
    clock clock{};
    prev_time_point = clock.now();

    main_window_->open();
    while (main_window_->is_open())
    {
        time_point cur_time_point = clock.now();
        Game::time_delta = cur_time_point - prev_time_point;
        prev_time_point = cur_time_point;
        if (player_action_finished_)
        {
            turn_timer_->freeze();
        }

        for (uint32_t events_launched = 0; events_launched < EVENTS_HANDLING_PER_FRAME; ++events_launched)
        {   
            emanager_->process_external_events(main_window_);
        }

        Event time_event;
        time_event.set_type(EventType::TIME_PASSED);
        time_event.dt_ = Game::time_delta;
        emanager_->handle_event(time_event);
        main_window_->redraw(camera_->get_position());
        Event check_stability_event;
        check_stability_event.set_type(EventType::STABILITY_EVENT);
        is_stable_ = !emanager_->handle_event(check_stability_event);
        if (is_stable_ && (player_action_finished_ || turn_timer_->expired()))
        {
            pass_turn_();
        }
    }
}

void Game::add_to_map_children(AbstractNode *object)
{
    assert(object != nullptr);

    map_->add_child(object);
}

bool Game::launch_event(const Event &event)
{
    return emanager_->handle_event(event);
}

bool Game::is_under_control(const AbstractNode *object) const
{
    return under_control_ == object;
}

bool Game::player_action_finished() const
{
    return player_action_finished_;
}

void Game::finish_player_action()
{
    player_action_finished_ = true;
}

void Game::enable_player_action()
{
    player_action_finished_ = false;
}

bool Game::does_player_have_control() const
{
    return player_has_control_;
}

void Game::set_player_control(bool whether_has_control)
{
    player_has_control_ = whether_has_control;
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

bool Game::is_stable() const
{
    return is_stable_;
}

void Game::lock_camera() const
{
    camera_->lock();
}

void Game::set_character_under_control(const Character *now_under_control)
{
    under_control_ = now_under_control;
}

void Game::set_camera_tracking_object(const PhysicsObject *now_tracked_object)
{
    camera_tracking_ = now_tracked_object;
    camera_->lock();
}

void Game::pass_turn_()
{   
    uint32_t prev_active_team_index = active_team_index_;

    ++active_team_index_;
    active_team_index_ %= TEAMS_QUANTITY;

    while (!teams_[active_team_index_]->is_alive())
    {
        ++active_team_index_;
        active_team_index_ %= TEAMS_QUANTITY;
    }
    if (active_team_index_ == prev_active_team_index)
    {
        // game over
    }

    under_control_ = teams_[active_team_index_]->get_next_character();
    set_camera_tracking_object(under_control_);

    enable_player_action();
    set_player_control(true);

    turn_timer_->restart();
    turn_timer_->resume();
}
