#ifndef IMAGE_HPP
#define IMAGE_HPP


#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include "rect.hpp"


class Image
{
    friend class Texture;

    class Image_ : public sf::Image
    {};


    Image(const sf::Image &image, uint32_t x, uint32_t y)
      : image_(new Image_())
    {
        image_->copy(image, x, y);
        printf("image, after constructing: %u %u\n", image_->getSize().x, image.getSize().y);
    }

public:
//----------------------------------------------------------------------------------
    Image()
      : image_(new Image_())
    {}

    ~Image()
    {
        delete image_;
    }
//----------------------------------------------------------------------------------
    bool load_from_file(const std::string &file_name)
    {
        return image_->loadFromFile(file_name);
    }

    void create(uint32_t width, uint32_t height, uint32_t color = 0x000000ff)
    {
        image_->create(width, height, sf::Color(color));
    }

    void copy(const Image &other, uint32_t x, uint32_t y, const Rect<int> &area = Rect<int>())
    {
        image_->copy(*other.image_, x, y, area.to_SFML_rect());
    }

    uint32_t get_pixel(uint32_t x, uint32_t y) const
    {
        uint32_t result = image_->getPixel(x, y).toInteger();
        uint8_t *result_ptr = reinterpret_cast<uint8_t *>(&result);     // reversing result bytes
        uint8_t temp = 0;

        temp = *result_ptr;
        *result_ptr = *(result_ptr + 3);
        *(result_ptr + 3) = temp;

        temp = *(result_ptr + 1);
        *(result_ptr + 1) = *(result_ptr + 2);
        *(result_ptr + 2) = temp;

        return result;
    }

    void set_pixel(uint32_t x, uint32_t y, uint32_t color)
    {
        sf::Color pixel_color(color);
        image_->setPixel(x, y, pixel_color);
    } 

    const uint8_t *get_pixels_ptr() const
    {
        return image_->getPixelsPtr();
    }


    uint32_t get_width() const
    {
        return image_->getSize().x;
    }

    uint32_t get_height() const
    {
        return image_->getSize().y;
    }

private:
public:
    Image_ *image_;
};

#endif
