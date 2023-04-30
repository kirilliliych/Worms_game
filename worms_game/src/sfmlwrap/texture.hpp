#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <SFML/Graphics/Texture.hpp>
#include "rect.hpp"
#include "image.hpp"

class Sprite;
class Surface;

class Texture
{
    friend class Sprite;
    friend class Surface;

    class Texture_ : public sf::Texture
    {
    public:

        Texture_()
          : sf::Texture()
        {}

        Texture_(const Texture_ &other)
          : sf::Texture(other)
        {}
    };

public:

    Texture();

    // Texture(const std::string &file_name, const Rect<int> &area = Rect<int>());

    ~Texture();

    bool create(uint32_t width, uint32_t height);

    bool load_from_file(const std::string &file_name, const Rect<int> &area = Rect<int>());

    bool load_from_image(const Image &image, const Rect<int> &area = Rect<int>());

    Image copy_to_image() const;

    void update(const uint8_t *pixels, uint32_t width, uint32_t height, uint32_t x, uint32_t y);

    void set_repeated(bool repeated);

    bool is_repeated() const;

private:
public:
    Texture_ *texture_;
};


#endif
