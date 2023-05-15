#include <chrono>
#include <cstdlib>
#include <ctime>
#include <ratio>


#include <SFML/Graphics.hpp>
#include "game.hpp"

// TODO: erase лишнего в отдельный ивент, no unique_ptr?;

int main()
{
    std::srand(std::time(nullptr));

    Game game(1200, 800,
                        1920, 1080);
    game.run();

    // std::vector<int> v(7);
    // for (int i = 0; i < 7; ++i)
    // {
    //     v[i] = i + 1;
    // }
    // for (int i = 0; i < v.size(); ++i)
    // {
    //     std::cout << v[i] << " ";
    // }
    // std::cout << std::endl;
    // v.erase(std::remove_if(v.begin(), v.end(), [](int val) {return val % 2 == 0;}), v.end());
    // for (int i = 0; i < v.size(); ++i)
    // {
    //     std::cout << v[i] << " ";
    // }
    // std::cout << std::endl <<  "size: " << v.size() << std::endl;


    // std::chrono::system_clock clock{};                                          // example
    // std::chrono::time_point<std::chrono::system_clock> p1 = clock.now();
    // for (int i = 0; i < 200; ++i)
    // {}
    // std::chrono::time_point<std::chrono::system_clock> p2 = clock.now();
    // std::chrono::duration<float, std::micro> a = p2 - p1;
    // std::cout << a.count() << std::endl;


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
