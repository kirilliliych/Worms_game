#pragma once


#include "sfmlwrap/texture.hpp"
#include "utilities.hpp"
#include <SFML/Graphics.hpp>
#include "sfmlwrap/image.hpp"
#include <cassert>
#include <filesystem>
#include <ostream>
#include <unordered_map>


class ImageManager
{
public:

    ImageManager()
      : images_()
    {
        put_textures_to_image_manager_();
    }

    ~ImageManager()
    {
        auto images_iterator = images_.begin();
        auto images_end = images_.cend();
        while (images_iterator != images_end)
        {
            delete images_iterator->second;

            ++images_iterator;
        }
    }

    void add_image(const Image *image, const std::string &file_name)
    {
        images_[file_name] = image;
        printf("%s added to map\n", file_name.c_str());
    }

    const Image *get_image(const std::string &file_name)
    {
        const Image *result = images_[file_name];
        // std::cout << "get_image: filename is " << file_name << std::endl;

        return result;
    }

private:

void put_textures_to_image_manager_()
{
    for (const auto &file : std::filesystem::directory_iterator("./worms_game/images/"))
    {
        auto cur_path = file.path();
        std::string p = cur_path;
        if (!p.ends_with("sprite_sheet.png"))
        {
            Image *cur_image = new Image();
            std::cout << "trying to load " << cur_path << std::endl;
            bool loading_result = cur_image->load_from_file(cur_path);
            assert(loading_result);

            // std::cout << "putting image with filename " << cur_path.filename() << " to map" << std::endl;
            add_image(cur_image, cur_path.filename());
        }
    }

    cropping_function();
}

void extract_part_of_image_(const Image &sprite_sheet, const Rect<int> &area, const std::string &image_file_name)
{
    Texture proxy_texture;
    proxy_texture.load_from_image(sprite_sheet, area);
    Image *image = new Image();
    image->create(area.width(), area.height());
    image->copy(proxy_texture.copy_to_image(), 0, 0); // after copying all pixels that were not explicitly changed by copying are changed to ff000000!
    const uint32_t *pixels = reinterpret_cast<const uint32_t *> (image->get_pixels_ptr());
    uint32_t background_color = pixels[0];
    // uint8_t *ptr = reinterpret_cast<uint8_t *> (&background_color);
    // uint8_t temp = *ptr;
    // *ptr = *(ptr + 3);
    // *(ptr + 3) = temp;
    // temp = *(ptr + 1);
    // *(ptr + 1) = *(ptr + 2);
    // *(ptr + 2) = temp;
    for (uint32_t y = 0; y < image->get_height(); ++y)
    {
        for (uint32_t x = 0; x < image->get_width(); ++x)
        {
            uint32_t cur_pixel = pixels[y * image->get_width() + x];
            if ((cur_pixel == 0xff000000) || (cur_pixel == reverse_bytes(background_color)))
            {
                image->set_pixel(x, y, 0x00000000);
            }
        }
    }
    add_image(image, image_file_name);
}


void cropping_function()
{
    Image worms_sprite_sheet_image;
    assert(worms_sprite_sheet_image.load_from_file("./worms_game/images/worms_sprite_sheet.png"));
    extract_part_of_image_(worms_sprite_sheet_image, {23, 32, {7, 7}}, "standing.png");

    extract_part_of_image_(worms_sprite_sheet_image, {23, 32, {600, 46}}, "rocket_launcher_left.png");
    extract_part_of_image_(worms_sprite_sheet_image, {23, 32, {639, 46}}, "rocket_launcher_up.png");
    extract_part_of_image_(worms_sprite_sheet_image, {23, 32, {673, 46}}, "rocket_launcher_left_up.png");
    extract_part_of_image_(worms_sprite_sheet_image, {23, 32, {710, 46}}, "rocket_launcher_left_down.png");
    extract_part_of_image_(worms_sprite_sheet_image, {23, 32, {745, 46}}, "rocket_launcher_down.png");
    extract_part_of_image_(worms_sprite_sheet_image, {25, 30, {123, 378}}, "rocket.png");

    Image debris_base_image;
    assert(debris_base_image.load_from_file("./worms_game/images/dirt2.png"));
    extract_part_of_image_(debris_base_image, {8, 8, {0, 0}}, "debris.png");
}

private:

    std::unordered_map<std::string, const Image *> images_;
};
