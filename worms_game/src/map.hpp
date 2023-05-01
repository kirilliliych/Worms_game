#ifndef MAP_HPP
#define MAP_HPP


#include <SFML/Window/Event.hpp>
#include <cstdlib>
#include <vector>
#include "abstract_node.hpp"
#include "game.hpp"
#include "sfmlwrap/events/base_event.hpp"
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
    enum MapPixelCondition
    {
        SKY = 0,
        TERRAIN
    };

public:
//---------------------------------------------------------------------------------
    Map(AbstractNode *parent, uint32_t width, uint32_t height)
      : AbstractNode(parent, Rect<int>(static_cast<int> (width),
                                       static_cast<int> (height),
                                       {0, 0})),
        width_(width),
        height_(height),
        map_(width * height, 0),
        landscape_images_(string_consts::landscape_images_names_pool.size())
    {
        id = 1;

        uint64_t landscape_types_quantity = string_consts::landscape_images_names_pool.size();
        for (uint64_t i = 0; i < landscape_types_quantity; ++i)
        {
            landscape_images_[i] = Game::imanager.get_image(string_consts::landscape_images_names_pool[i]);
        }

        texture_->create(width, height);
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
        std::vector<float> noise_seed(width_, 0);
        for (uint32_t i = 1; i < width_; ++i)
        {
            noise_seed[i] = static_cast<float> (std::rand()) / static_cast<float> (RAND_MAX);
        }
        noise_seed[0] = 0.5f;   // means terrain starts and ends halfway up due to algorithm implementation

        std::vector<float> surface = PerlinNoise1D_(noise_seed, 8, 2.0f);
        for (uint32_t y = 0; y < height_; ++y)
        {
            for (uint32_t x = 0; x < width_; ++x)
            {
                map_[(height_ - 1 - y) * width_ + x] = y >= surface[x] * height_ ? MapPixelCondition::SKY :
                                                                                   MapPixelCondition::TERRAIN;  // currently there are two types of landscape...
            }
        }

        update_map_texture_();
    }

    bool handle_event(const BaseEvent &event)
    {
        switch (event.get_type())
        {
            // pupupu skip

            case EventType::EXPLOSION_EVENT:
            {
                for (uint32_t i = 0; i < children_.size(); ++i)
                {
                    children_[i]->handle_event(event);
                }

                // make_crater(radius);
            }
        }
    } 

private:

    void update_map_texture_()      // ВНИМАНИЕ: ЦВЕТ, ВЫТАСКИВАЕМЫЙ ЧЕРЕЗ to_Integer ИЗ sf::Color, НЕКОРРЕКТЕН! ТРЕБУЕТСЯ ПОБАЙТОВОЕ РАЗВОРАЧИВАНИЕ
    {
        uint64_t pixels_size = width_ * height_ * sizeof(uint32_t) / sizeof(uint8_t);
        std::vector<uint8_t> pixels(pixels_size);
        uint32_t *pixels_data = reinterpret_cast<uint32_t *> (pixels.data());

        for (uint32_t y = 0; y < height_; ++y)
        {
            for (uint32_t x = 0; x < width_; ++x)
            {
                switch (map_[y * width_ + x])
                {
                    case MapPixelCondition::SKY:                                                                                // copypaste?
                    {
                        uint32_t image_width  = landscape_images_[MapPixelCondition::SKY]->get_width();
                        uint32_t image_height = landscape_images_[MapPixelCondition::SKY]->get_height();
                        pixels_data[y * width_ + x] = landscape_images_[MapPixelCondition::SKY]->get_pixel(x % image_width,
                                                                                                           y % image_height);
                        break;
                    }

                    case MapPixelCondition::TERRAIN:
                    {
                        uint32_t image_width  = landscape_images_[MapPixelCondition::TERRAIN]->get_width();
                        uint32_t image_height = landscape_images_[MapPixelCondition::TERRAIN]->get_height();
                        pixels_data[y * width_ + x] = landscape_images_[MapPixelCondition::TERRAIN]->get_pixel(x % image_width,
                                                                                                               y % image_height);
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
        
        texture_->update(pixels.data(), width_, height_, 0, 0);
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

private:
//-----------------------------------Variables-------------------------------------
    uint32_t width_;
    uint32_t height_;
    std::vector<uint8_t> map_;

    std::vector<const Image *> landscape_images_;
};

#endif
