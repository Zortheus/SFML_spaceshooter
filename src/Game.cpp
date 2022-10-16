#include "headers/Game.h"

// Initializers
void Game::initWindow()
{
    this->window = new sf::RenderWindow(sf::VideoMode(800, 600), "Space Shooter", sf::Style::Close | sf::Style::Titlebar);
    this->window->setFramerateLimit(144);
    this->window->setVerticalSyncEnabled(false);
}

void Game::initTextures()
{
    this->textures["BULLET"] = new sf::Texture();
    this->textures["BULLET"]->loadFromFile("textures/bullet.png");
}

void Game::initPlayer()
{
    this->player = new Player();
}

void Game::initEnemies()
{
    this->spawnTimerMax = 50.0f;
    this->spawnTimer = this->spawnTimerMax;
    //this->enemy = new Enemy(20.f, 20.f);
}

// Constructors/Destructors
Game::Game()
{
    this->initWindow();
    this->initTextures();
    this->initPlayer();
    this->initEnemies();
}

Game::~Game()
{
    delete this->window;
    delete this->player;
    
    // Delete textures - prevent memory leaks
    for (auto &i : this->textures)
    {
        delete i.second; // second part of map contains texture - delete them
    }

    // Delete bullets
    for (auto *i : this->bullets)
    {
        delete i;
    }

    // Delete enemies
    for (auto *i : this->enemies)
    {
        delete i;
    }
}


// Functions
void Game::run()
{
    while(this->window->isOpen())
    {
        this->update();
        this->render();
    }
}

void Game::updatePollEvents()
{
    sf::Event e;
    while(this->window->pollEvent(e))
    {
        if(e.Event::type == sf::Event::Closed)
            this->window->close();
        if(e.Event::KeyPressed && e.Event::key.code == sf::Keyboard::Escape)
            this->window->close();
    }
}

void Game::updateInput()
{
    // Move player
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        this->player->move(-1.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        this->player->move(1.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        this->player->move(0.f, -1.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        this->player->move(0.f, 1.f);

    // update bullets
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->player->canAttack())
    {
        // create bullet when mouse is pressed
        this->bullets.push_back(
            new Bullet(
            this->textures["BULLET"], 
            this->player->getPos().x + this->player->getBounds().width/2.f,
            this->player->getPos().y, 
            0.f, -1.f, 5.f));
    }
}

void Game::updateBullets()
{
    unsigned counter = 0;
    for (auto *bullet : this->bullets)
    {
        bullet->update();

        // Bullet culling (top of screen)
        if (bullet->getBounds().top + bullet->getBounds().height < 0.f)
        {
            // Delete current bullet
            delete this->bullets.at(counter);
            this->bullets.erase(this->bullets.begin() + counter);
            --counter;
            
            // check if bullets are being deleted
            //std::cout << this->bullets.size() << "\n";
        }

        ++counter; // when used alone, optimizes
    }
}

void Game::updateEnemies()
{
    this->spawnTimer += 0.5f;
    if (this->spawnTimer >= this->spawnTimerMax)
    {
        this->enemies.push_back(new Enemy(rand()%200, rand()%200));
        this->spawnTimer = 0.f;
    }

    for (auto *enemy : this->enemies)
    {
        enemy->update();
    }
}

void Game::update()
{
    this->updatePollEvents();
    this->updateInput();
    this->player->update();
    this->updateBullets();
    this->updateEnemies();
}

void Game::render()
{
    this->window->clear();

    //draw
    this->player->render(*this->window);

    for (auto *bullet : this->bullets)
    {
        bullet->render(this->window);
    }

    for (auto *enemy : this->enemies)
    {
        enemy->render(this->window);
    }

    this->window->display();
}