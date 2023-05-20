#pragma once


#include <SFML/Graphics.hpp>


class Font
{
    friend class Text;

    class Font_ : public sf::Font
    {};

public:

    Font()
      : font_(new Font_())
    {}

    Font &operator =(const Font &other)
    {
        font_->operator =(*other.font_);

        return *this;
    }

    ~Font()
    {
        delete font_;
    }

    bool load_from_file(const std::string &file_name)
    {
        return font_->loadFromFile(file_name);
    }

private:
public:
    Font_ *font_;
};
