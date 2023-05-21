#include <SFML/Graphics.hpp>
#include "game.hpp"


// AI, fps and vector
int main()
{
    std::srand(std::time(nullptr));

    Game game(1200, 800,
                 1920, 1080);
    game.run();
    

    return 0;
}
