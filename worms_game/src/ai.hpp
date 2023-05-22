#pragma once


#include <cmath>
#include "game.hpp"
#include "maths.hpp"
#include "sfmlwrap/events/event.hpp"
#include "team.hpp"
#include "weapon_traits.hpp"


class AI
{
public:

    enum class AIState
    {
        ASSESSING_ENVIRONMENT,
        MOVING,
        CHOOSING_TARGET,
        CHOOSING_WEAPON,
        POSITION_FOR_TARGET,
        AIMING,
        FIRING
    };

    AI()
      : state_(AIState::ASSESSING_ENVIRONMENT),
        controlled_team_(nullptr),
        x_position_(0),
        target_character_(nullptr)
    {}

    void set_controlled_team(Team *team)
    {
        controlled_team_ = team;
    }

    void act()
    {
        switch (state_)
        {
            case AIState::ASSESSING_ENVIRONMENT:
            {
                x_position_ = Game::game->get_map_width() / 2;
                controlled_character_ = nullptr;
                target_character_ = nullptr;

                controlled_character_ = controlled_team_->get_cur_character();
                assert(controlled_character_ != nullptr);

                x_position_ = controlled_character_->get_area().left_x();
                int left_pos  = 20;
                int right_pos = Game::game->get_map_width() - 20;
                if (x_position_ <= left_pos)
                {
                    x_position_ = left_pos;
                }
                if (x_position_ >= right_pos)
                {
                    x_position_ = right_pos;
                }

                printf("AI passed ASSESSING_ENVIRONMENT state\n");
                state_ = AIState::MOVING;

                break;
            }

            case AIState::MOVING:   // empty with current passive strategy
            {
                // Character *controlled_character = controlled_team_->get_cur_character();
                // assert(controlled_character != nullptr);

                printf("AI passed MOVING state\n");
                state_ = AIState::CHOOSING_TARGET;

                break;
            }

            case AIState::CHOOSING_TARGET:
            {
                uint32_t target_team_index = std::rand() % Game::TEAMS_QUANTITY;
                Team *target_team = Game::game->get_team(target_team_index);
                while ((target_team == controlled_team_) || (!target_team->is_alive()))
                {
                    target_team_index = std::rand() % Game::TEAMS_QUANTITY;
                    target_team = Game::game->get_team(target_team_index);
                }

                target_character_ = target_team->get_healthiest_character();

                printf("AI passed CHOOSING_TARGET state\n");
                state_ = AIState::CHOOSING_WEAPON;

                break;
            }

            case AIState::CHOOSING_WEAPON:
            {
                int weapon_index = std::rand() % traits::weapon_traits_pool.size();
                assert(weapon_index == 0);

                Event weapon_choice_event;
                weapon_choice_event.set_type(EventType::KEY_PRESSED);
                weapon_choice_event.kedata_.key_code = KeyboardKey::Num1;
                Game::game->launch_event(weapon_choice_event);

                printf("AI passed CHOOSING_WEAPON state\n");
                state_ = AIState::POSITION_FOR_TARGET;

                break;
            }

            case AIState::POSITION_FOR_TARGET:
            {
                float dx = -(target_character_->get_area().center_x() - controlled_character_->get_area().center_x());
                float dy = -(target_character_->get_area().center_y() - controlled_character_->get_area().center_y());
                float init_speed = 1500.f;
                float init_speed_2 = init_speed * init_speed;
                float y_acceleration = 2000.f;

                float discriminant = init_speed_2 * init_speed_2 - y_acceleration * (y_acceleration * dx * dx + 2.f * dy * init_speed_2);
                
                if (discriminant < 0)
                {
                    printf("AI: target is out of range!\n");
                }
                else
                {
                    float root1 = init_speed_2 + sqrtf(discriminant);
                    float root2 = init_speed_2 - sqrtf(discriminant);

                    float denominator = y_acceleration * dx;
                    float angle1 = atanf(root1 / denominator);  // max height
                    float angle2 = atanf(root2 / denominator);  // min height

                    float target_angle = angle1 - (dx > 0 ? math_consts::PI : 0.f);
                    
                }

                printf("AI passed POSITION_FOR_TARGET state\n");
                state_ = AIState::AIMING;

                break;
            }
        }
    }

    AIState state_;
    Team *controlled_team_;

    int x_position_;
    Character *controlled_character_;
    Character *target_character_;
    float charge_;
};