#ifndef IMAGE_HPP
#define IMAGE_HPP


#include "SFML/Graphics.hpp"


class Image
{
    class Image_ : public sf::Image
    {
    
    };

public:
//----------------------------------------------------------------------------------
    Image()
      : image_(new Image_())
    {}
    
    ~Image()
    {}
//----------------------------------------------------------------------------------
    bool load_from_file(const std::string &file_name)
    {
        return image_->loadFromFile(file_name);
    }

    uint32_t get_pixel(uint32_t x, uint32_t y)
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

    uint32_t get_width() const
    {
        return image_->getSize().x;
    }

    uint32_t get_height() const
    {
        return image_->getSize().y;
    }

private:

    Image_ *image_;
};

#endif
