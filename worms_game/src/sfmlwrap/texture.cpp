#include "sfmlwrap/texture.hpp"
#include "stl.hpp"
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
    // printf("texture: copied image size: %u %u\n", texture_image.getSize().x, texture_image.getSize().y);

    Image result;
    result.create(texture_->getSize().x, texture_->getSize().y);
    result.image_->copy(texture_image, 0, 0);
    // printf("texture: result image size: %u %u\n", result.get_width(), result.get_height());
    
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

Point2d<uint32_t> Texture::get_center() const
{
    return {get_width() / 2, get_height() / 2};
}

void Texture::fill_with_color(uint32_t color)
{
    uint32_t width  = get_width();
    uint32_t height = get_height();
    vector<uint32_t> pixels(width * height, 0);

    for (uint32_t cur_height = 0; cur_height < height; ++cur_height)
    {
        for (uint32_t cur_width = 0; cur_width < width; ++cur_width)
        {
            pixels[cur_height * width + cur_width] = color;
        }
    }

    texture_->update(reinterpret_cast<const uint8_t *> (pixels.data()), width, height, 0, 0);
}
