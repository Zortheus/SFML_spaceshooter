#ifndef GAME_H
#define GAME_H

#include <map> // container class that maps 2 things together; 
#include <string>
#include <sstream>
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

        // GUI
        sf::Font font;
        sf::Text pointText;
        sf::Text gameOverText;

        // World
        sf::Texture worldBackgroundTex;
        sf::Sprite worldBackground;

        // Systems
        unsigned points;
        float bgY = 0;
        
        // Player
        Player* player;

        // Player GUI
        sf::RectangleShape playerHpBar;
        sf::RectangleShape playerhpBarBack;

        // Enemies
        float spawnTimer;
        float spawnTimerMax;
        std::vector<Enemy*> enemies;

        // Private functions
        void initWindow();
        void initTextures();
        void initGUI();
        void initWorld();
        void initSystems();

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
        void updateGUI();
        void updateWorld();
        void updateCollision();
        void updateBullets();
        void updateEnemies();
        void updateCombat();
        void update();
        void renderGUI();
        void renderWorld();
        void render();
};

#endif