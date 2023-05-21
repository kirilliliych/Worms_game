#pragma once


#include <cassert>
#include "font.hpp"
#include "SFML/Graphics.hpp"


class Text
{
    friend class Surface;

    class Text_ : public sf::Text
    {};

public:

    Text()
      : text_(new Text_())
    {}

    ~Text()
    {
        delete text_;
    }

    void set_string(const char *str)
    {
        assert(str != nullptr);

        text_->setString(str);
    }

    bool font_load_from_file(const std::string &font_file_name)
    {
        if (!font_.load_from_file(font_file_name))
        {
            std::cerr << "ERROR: font " << font_file_name << " was not found" << std::endl;

            return false; 
        }
        text_->setFont(*font_.font_);

        return true;
    }

    void set_character_size(uint32_t size)
    {
        text_->setCharacterSize(size);
    }

    void set_fill_color(uint32_t color)
    {
        text_->setFillColor(sf::Color(color));
    }

    void set_position(int x, int y)
    {
        text_->setPosition(static_cast<float> (x),
                           static_cast<float> (y));
    }

private:
public:
    Font   font_;
    Text_ *text_;
};
