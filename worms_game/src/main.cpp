#include <SFML/Graphics.hpp>
#include "game.hpp"


// make turn, AI, fps and vector, colors
// out of bounds, character-projectile interaction
int main()
{
    std::srand(std::time(nullptr));

    Game game(1200, 800,
                 1920, 1080);
    game.run();
    

    return 0;
}
