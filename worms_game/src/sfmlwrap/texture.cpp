#include "sfmlwrap/texture.hpp"
#include "surface.hpp"


Texture::Texture()
  : texture_(new Texture_())
{}

Texture &Texture::operator =(const Texture &other)
{
    texture_->operator =(*other.texture_);

    return *this;
}

Texture::~Texture()
{
    delete texture_;
}

bool Texture::create(uint32_t width, uint32_t height)
{
    return texture_->create(width, height);
}

bool Texture::load_from_file(const std::string &file_name, const Rect<int> &area)
{
    return texture_->loadFromFile(file_name, area.to_SFML_rect());
}

bool Texture::load_from_image(const Image &image, const Rect<int> &area)
{
    return texture_->loadFromImage(*image.image_, area.to_SFML_rect());
}

Image Texture::copy_to_image() const
{
    sf::Image texture_image = texture_->copyToImage();
    printf("texture: copied image size: %u %u\n", texture_image.getSize().x, texture_image.getSize().y);

    Image result;
    result.create(150, 200);
    result.image_->copy(texture_image, 0, 0);
    printf("texture: result image size: %u %u\n", result.get_width(), result.get_height());
    
    return result;
}

void Texture::update(const uint8_t *pixels, uint32_t width, uint32_t height, uint32_t x, uint32_t y)
{
    assert(pixels != nullptr);
    texture_->update(pixels, width, height, x, y);
}

void Texture::set_repeated(bool repeated)
{
    texture_->setRepeated(repeated);
}

bool Texture::is_repeated() const
{
    return texture_->isRepeated();
}

uint32_t Texture::get_width() const
{
    return texture_->getSize().x;
}

uint32_t Texture::get_height() const
{
    return texture_->getSize().y;
}
