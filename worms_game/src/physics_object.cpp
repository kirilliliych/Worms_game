// #include "game.hpp"
// #include "map.hpp"
// #include "physics_entity.hpp"
#include "physics_object.hpp"


PhysicsObject::PhysicsObject(AbstractNode *parent, const Rect<int> &area, const Vector2d<float> &velocity,
                             const Vector2d<float> &acceleration, float friction, int bounces_before_death)
  : AbstractNode(parent, area),
    type_(PhysicsEntity::UNKNOWN),
    velocity_(velocity),
    acceleration_(acceleration),
    friction_(friction),
    bounces_before_death_(bounces_before_death),
    is_stable_(true),
    radius_(std::max(area.get_width(), area.get_height()))
{}

// PhysicsObject::PhysicsObject(AbstractNode *parent, const Rect<int> &area, const Vector2d<float> &velocity,
//                              const Vector2d<float> &acceleration, float friction, int bounces_before_death,
//                              const std::string &texture_file_name, const Rect<int> &texture_area)
//   : AbstractNode(parent, area, texture_file_name, texture_area),
//     type_(PhysicsEntity::UNKNOWN),
//     velocity_(velocity),
//     acceleration_(acceleration),
//     friction_(friction),
//     bounces_before_death_(bounces_before_death),
//     is_stable_(true),
//     radius_(std::max(area.get_width(), area.get_height()))
// {}

// PhysicsEntity PhysicsObject::get_physics_entity_type() const
// {
//     return type_;
// }

// bool PhysicsObject::handle_event(const Event &event)
// {
//     bool result = false;
//     switch (event.get_type())
//     {
//         case EventType::TIME_PASSED:
//         {
//             acceleration_.set_y(800);
//             velocity_ += acceleration_ * Game::time_delta.count();
//             if (this == reinterpret_cast<const PhysicsObject *> (Game::game->get_character_under_control()))
//             {
//                 // if (a)
//                 // if (velocity_.y() > 1000)
//                 // {
//                 // printf("under control velocity: %g %g\n", velocity_.x(), velocity_.y());
//                 // }
//             }
//             float new_x = area_.left_top().x() + velocity_.x() * Game::time_delta.count();
//             float new_y = area_.left_top().y() + velocity_.y() * Game::time_delta.count();
//             if (a)
//             {
//                 // printf("new_x: %g = left topx: %d + velocityx: %g * time_delta: %g\n", new_x, area_.left_top().x(), velocity_.x(), Game::time_delta.count());
//                 // printf("new_y: %g = left topy: %d + velocityy: %g * time_delta: %g\n", new_y, area_.left_top().y(), velocity_.y(), Game::time_delta.count());
//             }
//             a = false;
//             acceleration_.set_x(0);
//             acceleration_.set_y(0);
//             is_stable_ = false;
//             float angle = atan2f(velocity_.x(), velocity_.y());
//             bool collision_happened = false;
//             float response_x = 0;
//             float response_y = 0;
//             for (float r = angle - 3.14159f / 2.0f; r < angle + 3.14159f / 2.0f; r += 3.14159f / 8.0f)  // definitely need better collision checking
//             {
//                 float test_pos_x = radius_ * cosf(r) + new_x;
//                 float test_pos_y = radius_ * sinf(r) + new_y;
//                 if (test_pos_x >= Game::game->get_map_width())
//                 {
//                     test_pos_x = Game::game->get_map_width() - 1;
//                 }
//                 if (test_pos_y >= Game::game->get_map_height())
//                 {
//                     test_pos_y = Game::game->get_map_height() - 1;
//                 }
//                 if (test_pos_x < 0)
//                 {
//                     test_pos_x = 0;
//                 }
//                 if (test_pos_y < 0)
//                 {
//                     test_pos_y = 0;
//                 }
                
//                 if (Game::game->check_collision(this, get_physics_entity_type(),
//                                                  {static_cast<int> (test_pos_x),
//                                                  static_cast<int> (test_pos_y)}))
//                 {
//                     response_x += new_x - test_pos_x;
//                     response_y += new_y - test_pos_y;
//                     collision_happened = true;

//                     // Game::game->map_->map_[test_pos_y * Game::game->get_map_width() + test_pos_x] =  Game::game->map_->COLLISION;
//                     // Game::game->map_->update_map_texture_();
//                 }
//             }
//             float velocity_magnitude = sqrtf(velocity_.x() * velocity_.x() +
//                                                 velocity_.y() * velocity_.y());
//             if ((collision_happened) && !is_stable_)
//             {
//                 is_stable_ = true;
                
//                 float response_magnitude = sqrtf(response_x * response_x +
//                                                     response_y * response_y);
                
//                 // reflection vector
//                 float response_x_norm = response_x / response_magnitude;
//                 float response_y_norm = response_y / response_magnitude;
//                 float dot = velocity_.x() * (response_x_norm) +
//                             velocity_.y() * (response_y_norm);
//                 velocity_.set_x((-2.0f * dot * response_x_norm + velocity_.x()) * friction_);
//                 velocity_.set_y((-2.0f * dot * response_y_norm + velocity_.y()) * friction_);
//                 // printf("")
//                 // printf("after collision velocity is %g %g\n", velocity_.x(), velocity_.y());
//                 if (bounces_before_death_ > 0)
//                 {
//                     --bounces_before_death_;
//                     exists_ = bounces_before_death_ > 0;
//                     if (!exists_)
//                     {
//                         on_bounce_death(area_.left_top() + Point2d<int>(area_.get_width() / 2, area_.get_height() / 2));
//                     }
//                 }
//             }
//             else
//             {
//                 area_.set_left_top_x(new_x);
//                 area_.set_left_top_y(new_y);
//             }
//             if (velocity_magnitude < 50.f)
//             {
//                 // printf("set to true\n");
//                 is_stable_ = true;
//                 // velocity_.set_x(0);
//                 // velocity_.set_y(0);
//             }
//             // result = children_handle_event(event);
//             for (uint32_t child_index = 0; child_index < children_.size(); ++child_index)
//             {
//                 if (children_[child_index]->handle_event(event))
//                 {
//                     result = true;
//                 }
//             }
//             break;
//         }
//         default:
//         {
//             for (uint32_t child_index = 0; child_index < children_.size(); ++child_index)
//             {
//                 if (children_[child_index]->handle_event(event))
//                 {
//                     result = true;
//                 }
//             }
//             // result = children_handle_event(event);
//         }
//     }
//     return result;
// }