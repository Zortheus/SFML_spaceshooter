#include "headers/Game.h"
#include <time.h>

int main()
{
    srand(static_cast<unsigned>(time(NULL)));

    Game game;
    
    game.run();

    return 0;
}