// #include "physics_object.hpp"

// Character(AbstractNode *parent, const Rect<int> &area)    // do we need this?
//       : PhysicsObject(parent, area, {0, 100}, {0, 0}, DEFAULT_FRICTION, -1),
//         Collidable(CollidableEntity::CHARACTER),
//         texture_scale_(1),
//         hp_(100),
//         is_alive_(true),
//         init_falling(true)
//     {
//         PhysicsObject::type_ = PhysicsEntity::CHARACTER;
//     }

//     Character(AbstractNode *parent, const Rect<int> &area, const std::string &texture_file_name,
//               const Rect<int> &texture_area = Rect<int>())
//       : PhysicsObject(parent, area, {0, 100}, {0, 0},
//                       DEFAULT_FRICTION, -1, texture_file_name, texture_area),
//         Collidable(CollidableEntity::CHARACTER),
//         texture_scale_(1),
//         hp_(100),
//         is_alive_(true),
//         init_falling(true)
//     {
//         PhysicsObject::type_ = PhysicsEntity::CHARACTER;

//         uint32_t texture_width  = texture_->get_width();
//         uint32_t texture_height = texture_->get_height();
//         assert(area.get_width()  >= 0);
//         assert(area.get_height() >= 0);
//         float asked_width  = static_cast<float> (area.get_width());
//         float asked_height = static_cast<float> (area.get_height());
//         float x_scale = asked_width  / static_cast<float> (texture_width);
//         float y_scale = asked_height / static_cast<float> (texture_height);
//         if (y_scale < x_scale)
//         {
//             area_.set_width(y_scale * static_cast<int> (texture_width));
//             texture_scale_ = y_scale;
//         }
//         else
//         {
//             area_.set_height(x_scale * static_cast<int> (texture_height));
//             texture_scale_ = x_scale;
//         }
//     }

//     void on_bounce_death(const Point2d<int> &death_position) override
//     {}

//     void render_self(Surface *surface, const Point2d<int> &camera_offset) override
//     {
//         assert(surface != nullptr);

//         Sprite self_sprite(*texture_, area_.left_top() - camera_offset);
//         self_sprite.set_scale(texture_scale_, texture_scale_);
//         surface->draw_sprite(self_sprite);
//     }

//     bool handle_event(const Event &event) override
//     {
//         bool result = false;

//         switch (event.get_type())
//         {
//             case EventType::KEY_PRESSED:
//             {
//                 printf("CHARACTER CATCHES KEY AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
//                 if ((this == Game::game->get_character_under_control()) && 
//                     (is_stable_))
//                 {
//                     switch (event.kedata_.key_code)
//                     {
//                         case KeyboardKey::Z:
//                         {
//                             velocity_.set_x(-120.0f);
//                             velocity_.set_y(-160.0f);
//                             is_stable_ = false;

//                             break;
//                         }
                        
//                         case KeyboardKey::X:
//                         {
//                             velocity_.set_x(+120.0f);
//                             velocity_.set_y(-160.0f);
//                             is_stable_ = false;

//                             break;
//                         }

//                         default:
//                         {
//                             break;
//                         }
//                     }
//                 }
//             }

//             case EventType::COLLISION_EVENT:
//             {
//                 if ((event.cedata_.checker != PhysicsEntity::CHARACTER) &&
//                     (event.cedata_.checker != PhysicsEntity::DEBRIS)    &&
//                     (area_.contains(event.cedata_.position)))
//                 {
//                     result = true;
//                 }

//                 for (uint32_t child_index = 0; child_index < children_.size(); ++child_index)
//                 {
//                     if (children_[child_index]->handle_event(event))
//                     {
//                         result = true;
//                     }
//                 }
//                 // result = children_handle_event(event);

//                 break;
//             }

//             case EventType::EXPLOSION_EVENT:
//             {
//                 float center_x = area_.left_top().x() + static_cast<float> (area_.get_width())  / 2;
//                 float center_y = area_.left_top().y() + static_cast<float> (area_.get_height()) / 2;
//                 float dx = center_x - event.eedata_.position.x();
//                 float dy = center_y - event.eedata_.position.y();
//                 float distance = sqrtf(dx * dx + dy * dy);
//                 // printf("dx: %g dy: %g distance: %g\n", dx, dy, distance);
//                 if (distance < 0.001f)
//                 {
//                     // printf("distance too small\n");
//                     distance = 0.001f;
//                 }
//                 if (distance < event.eedata_.radius)
//                 {
//                     velocity_.set_x(3 * dx / (distance - std::min(area_.get_width() / 2, area_.get_height() / 2)) * event.eedata_.radius);
//                     velocity_.set_y(3 * dy / (distance - std::min(area_.get_width() / 2, area_.get_height() / 2)) * event.eedata_.radius);
//                     // printf("velocity: x %g y %g\n", velocity_.x(), velocity_.y());

//                     is_stable_ = false;
//                 }

//                 for (uint32_t i = 0; i < 20; ++i)
//                 {
//                     // due to add_to_map_children call map becomes parent of debris
//                     Game::game->add_to_map_children(new Debris(nullptr, {8, 8, event.eedata_.position},
//                                                     "debris.png"));
//                 }

//                 if (PhysicsObject::handle_event(event))
//                 {
//                     result = true;
//                 }

//                 for (uint32_t child_index = 0; child_index < children_.size(); ++child_index)
//                 {
//                     if (children_[child_index]->handle_event(event))
//                     {
//                         result = true;
//                     }
//                 }
//                 // result = children_handle_event(event);      // double handling

//                 break;
//             }

//             case EventType::TIME_PASSED:
//             {
//                 if (PhysicsObject::handle_event(event))
//                 {
//                     result = true;
//                 }

//                 for (uint32_t child_index = 0; child_index < children_.size(); ++child_index)
//                 {
//                     if (children_[child_index]->handle_event(event))
//                     {
//                         result = true;
//                     }
//                 }
//                 // result = children_handle_event(event);

//                 break;
//             }

//             default:
//             {
//                 for (uint32_t child_index = 0; child_index < children_.size(); ++child_index)
//                 {
//                     if (children_[child_index]->handle_event(event))
//                     {
//                         result = true;
//                     }
//                 }
//                 // result = children_handle_event(event);
//             }
//         }

//         return result;
//     }