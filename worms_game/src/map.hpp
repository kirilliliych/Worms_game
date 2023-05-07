#ifndef MAP_HPP
#define MAP_HPP


#include <algorithm>
#include <cstdlib>
#include <vector>
#include "abstract_node.hpp"
#include "debris.hpp"
#include "game.hpp"
#include "physics_object.hpp"
#include "projectile.hpp"
#include "sfmlwrap/events/event.hpp"
#include "sfmlwrap/image.hpp"
#include "sfmlwrap/rect.hpp"
#include "sfmlwrap/texture.hpp"


namespace string_consts
{
    const std::vector<std::string> landscape_images_names_pool{"seamless_sky.png",
                                                               "dirt1.png"};
}


class Map : public AbstractNode
{
public:
    enum MapPixelCondition
    {
        SKY = 0,
        TERRAIN,
        COLLISION
    };

public:
//---------------------------------------------------------------------------------
    Map(AbstractNode *parent, const Rect<int> &area)
      : AbstractNode(parent, area),
        map_(static_cast<uint32_t> (area_.get_width() * area.get_height()), 0),
        landscape_images_(string_consts::landscape_images_names_pool.size())
    {
        uint64_t landscape_types_quantity = string_consts::landscape_images_names_pool.size();
        for (uint64_t i = 0; i < landscape_types_quantity; ++i)
        {
            landscape_images_[i] = Game::imanager.get_image(string_consts::landscape_images_names_pool[i]);
        }

        texture_->create(static_cast<uint32_t> (area.get_width()),
                         static_cast<uint32_t> (area_.get_height()));
    }

    Map(const Map &other) = delete;
    Map(Map &&other) = delete;

    Map &operator =(const Map &other) = delete;
    Map &operator =(Map &&other) = delete;

    ~Map()
    {}
//---------------------------------------------------------------------------------
    void create_map()
    {
        uint32_t width  = static_cast<uint32_t> (area_.get_width());
        uint32_t height = static_cast<uint32_t> (area_.get_height()); 

        std::vector<float> noise_seed(width, 0);
        for (uint32_t i = 1; i < width; ++i)
        {
            noise_seed[i] = static_cast<float> (std::rand()) / static_cast<float> (RAND_MAX);
        }
        noise_seed[0] = 0.5f;   // means terrain starts and ends halfway up due to algorithm implementation

        std::vector<float> surface = PerlinNoise1D_(noise_seed, 8, 2.0f);
        for (uint32_t y = 0; y < height; ++y)
        {
            for (uint32_t x = 0; x < width; ++x)
            {
                map_[(height - 1 - y) * width + x] = y >= surface[x] * height ? MapPixelCondition::SKY :
                                                                                MapPixelCondition::TERRAIN;  // currently there are two types of landscape...
            }
        }

        update_map_texture_();
    }

    uint32_t get_width() const
    {
        return area_.get_width();
    }

    uint32_t get_height() const
    {
        return area_.get_height();
    }

    bool handle_event(const Event &event)
    {
        bool result = false;

        switch (event.get_type())
        {
            case EventType::MOUSE_BUTTON_PRESSED:
            {
                switch (event.mbedata_.button)
                {
                    case MouseButton::LEFT:
                    {
                        Game::game->process_explosion(40, event.mbedata_.position);

                        break;
                    }

                    case MouseButton::RIGHT:
                    {
                        Game::game->add_to_map_children(new Projectile(this, {40, 40, event.mbedata_.position}, "rocket.png"));

                        break;
                    }

                    default:
                    {
                        break;
                    }
                }

                for (uint32_t child_index = 0; child_index < children_.size(); ++child_index)
                {
                    if (children_[child_index]->handle_event(event))
                    {
                        result = true;
                    }
                }
                // result = children_handle_event(event);

                break;
            }

            case EventType::EXPLOSION_EVENT:
            {
                printf("map explosion position: %d %d camera: %d %d\n", event.eedata_.position.x(), event.eedata_.position.y(), Game::game->get_camera_position().x(), Game::game->get_camera_position().y());
                make_crater_(event.eedata_.position, event.eedata_.radius);
                update_map_texture_();

                for (uint32_t child_index = 0; child_index < children_.size(); ++child_index)
                {
                    if (children_[child_index]->handle_event(event))
                    {
                        result = true;
                    }
                }

                // result = children_handle_event(event);

                break;
            }

            case EventType::COLLISION_EVENT:
            {
                if (map_[event.cedata_.position.y() * area_.get_width() + event.cedata_.position.x()] != MapPixelCondition::SKY)
                {
                    result = true;
                }

                for (uint32_t child_index = 0; child_index < children_.size(); ++child_index)
                {
                    if (children_[child_index]->handle_event(event))
                    {
                        result = true;
                    }
                }
                // result = children_handle_event(event);

                break;
            }

            case EventType::TIME_PASSED:
            {
                children_.erase(std::remove_if(children_.begin(),
                                                     children_.end(),
                                                     [](const std::unique_ptr<AbstractNode> &object) {return !object->does_exist();}),
                                children_.cend());

                for (uint32_t child_index = 0; child_index < children_.size(); ++child_index)
                {
                    if (children_[child_index]->handle_event(event))
                    {
                        result = true;
                    }
                }
                // result = children_handle_event(event);

                break;
            }

            default:
            {
                for (uint32_t child_index = 0; child_index < children_.size(); ++child_index)
                {
                    if (children_[child_index]->handle_event(event))
                    {
                        result = true;
                    }
                }
                // result = children_handle_event(event);
            }
        }

        return result;
    }

private:
public:
    void update_map_texture_()      // ВНИМАНИЕ: ЦВЕТ, ВЫТАСКИВАЕМЫЙ ЧЕРЕЗ to_Integer ИЗ sf::Color, НЕКОРРЕКТЕН! ТРЕБУЕТСЯ ПОБАЙТОВОЕ РАЗВОРАЧИВАНИЕ
    {
        uint32_t width  = static_cast<uint32_t> (area_.get_width());
        uint32_t height = static_cast<uint32_t> (area_.get_height()); 

        uint64_t pixels_size = width * height * sizeof(uint32_t) / sizeof(uint8_t);
        std::vector<uint8_t> pixels(pixels_size);
        uint32_t *pixels_data = reinterpret_cast<uint32_t *> (pixels.data());

        for (uint32_t y = 0; y < height; ++y)
        {
            for (uint32_t x = 0; x < width; ++x)
            {
                switch (map_[y * width + x])
                {
                    case MapPixelCondition::SKY:
                    {
                        uint32_t image_width  = landscape_images_[MapPixelCondition::SKY]->get_width();
                        uint32_t image_height = landscape_images_[MapPixelCondition::SKY]->get_height();
                        pixels_data[y * width + x] = landscape_images_[MapPixelCondition::SKY]->get_pixel(x % image_width,
                                                                                                          y % image_height);
                        break;
                    }

                    case MapPixelCondition::TERRAIN:
                    {
                        uint32_t image_width  = landscape_images_[MapPixelCondition::TERRAIN]->get_width();
                        uint32_t image_height = landscape_images_[MapPixelCondition::TERRAIN]->get_height();
                        pixels_data[y * width + x] = landscape_images_[MapPixelCondition::TERRAIN]->get_pixel(x % image_width,
                                                                                                              y % image_height);
                        break;
                    }

                    case MapPixelCondition::COLLISION:
                    {
                        pixels_data[y * width + x] = 0xff0000ff;
                        
                        break;
                    }

                    default:
                    {
                        std::cerr << "unknown pixel in map scheme texture" << std::endl;

                        break;
                    }
                }
            }
        }
        
        texture_->update(pixels.data(), width, height, 0, 0);
    }

    std::vector<float> PerlinNoise1D_(const std::vector<float> &noise_seed, int octaves_quantity, float bias)
	{
        int size = static_cast<int> (noise_seed.size());

        std::vector<float> result(size);
		for (int x = 0; x < size; x++)
		{
			float noise     = 0.0f;
			float scale_acc = 0.0f;
			float scale     = 1.0f;

			for (int cur_octave = 0; cur_octave < octaves_quantity; cur_octave++)
			{
				int pitch = size >> cur_octave;
				int sample1 = (x / pitch) * pitch;
				int sample2 = (sample1 + pitch) % size;
				float blend = (static_cast<float> (x - sample1)) / pitch;
				float sample = (1.0f - blend) * noise_seed[sample1] + blend * noise_seed[sample2];
				scale_acc += scale;
				noise += sample * scale;
				scale = scale / bias;
			}

			result[x] = noise / scale_acc;          // scaling to seed range
		}

        return result;
	}

    void make_crater_(const Point2d<int> &position, float radius)
    {
        auto CircleBresenham = [&](Point2d<int> position, int radius)
		{
			int x = 0;
			int y = radius;
			int p = 3 - 2 * radius;
			if (!radius) return;

			auto drawline = [&](int sx, int ex, int ny)
			{
				for (int i = sx; i < ex; i++)
					if (ny >= 0 && ny < area_.get_height() && i >= 0 && i < area_.get_width())
						map_[ny * area_.get_width() + i] = MapPixelCondition::SKY;
			};

			while (y >= x) 
			{
				drawline(position.x() - x, position.x() + x, position.y() - y);
				drawline(position.x() - y, position.x() + y, position.y() - x);
				drawline(position.x() - x, position.x() + x, position.y() + y);
				drawline(position.x() - y, position.x() + y, position.y() + x);
				if (p < 0) p += 4 * x++ + 6;
				else p += 4 * (x++ - y--) + 10;
			}
		};

		CircleBresenham(position, radius);
    }

private:
public:
//-----------------------------------Variables-------------------------------------
    std::vector<uint8_t> map_;

    std::vector<const Image *> landscape_images_;
};

#endif
