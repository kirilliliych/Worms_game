#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <cstdlib>
#include <ctime>

#include "game.hpp"


#include <SFML/Graphics.hpp>

int main()
{
    std::srand(std::time(nullptr));

    WormsGame game(1920, 1080,
                   1920, 1080);
    game.run();
}
