#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>

#include "game.hpp"


int main()
{
    std::srand(std::time(nullptr));

    WormsGame game(1920, 1080,
                   1920, 1080);
    game.run();
}
