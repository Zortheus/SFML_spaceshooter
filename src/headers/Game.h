#ifndef GAME_H
#define GAME_H

#include <map> // container class that maps 2 things together; 
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"

class Game
{
    private:
        // Window
        sf::RenderWindow* window;

        // Resources
        std::map<std::string, sf::Texture*> textures; //for each string, map a texture
        std::vector<Bullet*> bullets;

        // Player
        Player* player;

        // Enemies
        float spawnTimer;
        float spawnTimerMax;
        std::vector<Enemy*> enemies;
        

        // Private functions

        void initWindow();
        void initTextures();
        void initPlayer();
        void initEnemies();

    public:
        // Constructors/Destructors
        Game();
        virtual ~Game();

        //Functions
        void run();

        void updatePollEvents();
        void updateInput();
        void updateBullets();
        void updateEnemies();
        void update();
        void render();
};

#endif