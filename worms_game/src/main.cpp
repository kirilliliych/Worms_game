#include <chrono>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>

#include "game.hpp"
#include "sfmlwrap/texture.hpp"


// TODO: include guard через pragma; friend не класс, а отдельные функции

int main()
{
    std::srand(std::time(nullptr));

    // Game game(1920, 1080,
    //                1920, 1080);
    // game.run();



    // std::chrono::steady_clock clock{};                                          // example
    // std::chrono::time_point<std::chrono::steady_clock> p1 = clock.now();
    // for (int i = 0; i < 200000; ++i)
    // {}
    // std::chrono::time_point<std::chrono::steady_clock> p2 = clock.now();
    // auto diff = p2 - p1;
    // std::cout << diff.count() << std::endl;


    // sf::Texture texture;
    // texture.create(40, 60);
    // sf::Sprite sprite(texture);
    // sf::Vector2u v1 = sprite.getTexture()->getSize();
    // printf("v1: %u %u\n", v1.x, v1.y);
    // sprite.scale(2, 3);
    // sf::Vector2u v2 = sprite.getTexture()->getSize();
    // printf("v2: %u %u\n", v2.x, v2.y);


    // // sf::RenderWindow window(sf::VideoMode(800, 600), "works?");

    // Image worms_sprite_sheet_image;
    // assert(worms_sprite_sheet_image.load_from_file("worms_game/textures/worms_sprite_sheet.png"));
    // Texture temp;
    // temp.load_from_image(worms_sprite_sheet_image, {50, 40, {0, 0}});

    // Image itest;
    // // itest.image_->copy(*worms_sprite_sheet_image.image_, 0, 0);               // WHY DOES THIS NOT WORK?
    // itest = temp.copy_to_image();
    // printf("itestx: %u itesty: %u\n", itest.get_width(), itest.get_height());

    // // standing_image->image_->copy(temp.texture_->copyToImage(), 0, 0);
    // // standing_image->copy(temp.copy_to_image(), 0, 0);
    // // printf("added standing\n");
    // // // add_image(standing_image, "standing.png");
    // // t.load_from_image(*standing_image);
    // // t.texture_->loadFromImage(i);
    // // Sprite spr(t);
    // // while (window.isOpen())
    // // {
    // //     window.clear();
    // //     window.draw(*spr.sprite_);
    // //     window.display();
    // // }



    // // sf::Image i;                                     // works
    // // i = temp.texture_->copyToImage();
    // // sf::Vector2u i_size = i.getSize();
    // // printf("x: %u y: %u\n", i_size.x, i_size.y);
    // // Texture t;
    // // t.texture_->loadFromImage(i);
    // // Sprite spr(t);
    // // while (window.isOpen())
    // // {
    // //     window.clear();
    // //     window.draw(*spr.sprite_);
    // //     window.display();
    // // }
}
