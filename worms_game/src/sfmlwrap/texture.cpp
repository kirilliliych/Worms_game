#include "surface.hpp"

Texture::Texture()
  : texture_(new Texture_())
{}

Texture::Texture(const std::string &file_name, const Rect<int> &area)  // better to use default no-arg constructor and then load from file: you get info whether it finishes successfully
{
    load_from_file(file_name, area);
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
