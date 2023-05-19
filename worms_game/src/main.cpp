#include <SFML/Graphics.hpp>
#include "game.hpp"


// time counter, AI, fps and vector, animations, colors into another file, crosshair by mouse click, tombstone texture on character death
// out of bounds, character-projectile interaction
int main()
{
    std::srand(std::time(nullptr));

    Game game(1200, 800,
                 1920, 1080);
    game.run();
    

    return 0;
}
