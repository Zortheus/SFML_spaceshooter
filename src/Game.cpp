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

void Game::initGUI()
{
    // Load font
    if (!font.loadFromFile("fonts/Dosis-Light.ttf"))
        std::cout << "ERROR::GAME::Failed to load font" << "\n";

    //Init point text
    this->pointText.setPosition(700.f, 20.f);
    this->pointText.setFont(this->font);
    this->pointText.setCharacterSize(20);
    this->pointText.setFillColor(sf::Color::White);
    //this->pointText.setString("TEST");

    this->gameOverText.setFont(this->font);
    this->gameOverText.setCharacterSize(60);
    this->gameOverText.setFillColor(sf::Color::Red);
    this->gameOverText.setString("GAME OVER");
    this->gameOverText.setOrigin(this->gameOverText.getGlobalBounds().width / 2.f, this->gameOverText.getGlobalBounds().height / 2.f);
    this->gameOverText.setPosition(this->window->getSize().x / 2.f, this->window->getSize().y / 2.f);
    
    // Init player GUI
    this->playerHpBar.setSize(sf::Vector2f(300.f, 25.f));
    this->playerHpBar.setFillColor(sf::Color::Red);
    this->playerHpBar.setPosition(sf::Vector2f(20.f, 20.f));
    //this->playerHpBar.setOutlineColor

    this->playerhpBarBack = this->playerHpBar;
    this->playerhpBarBack.setFillColor(sf::Color(25, 25, 25, 200));
}

void Game::initWorld()
{
    if (!this->worldBackgroundTex.loadFromFile("textures/background1.jpeg"))
        std::cout << "ERROR::GAME::Could not load background texture" << "\n";

    this->worldBackgroundTex.setSmooth(false);
    this->worldBackgroundTex.setRepeated(true);
    this->worldBackground.setTexture(this->worldBackgroundTex);

    this->worldBackground.setTextureRect(sf::IntRect(0, bgY, 800, 600));
}

void Game::initSystems()
{
    this->points = 0;
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
    this->initGUI();
    this->initWorld();
    this->initSystems();

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
        this->updatePollEvents();
        
        if (this->player->getHp() > 0)
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

void Game::updateGUI()
{
    std::stringstream ss;
    ss << "POINTS : " << this->points;
    this->pointText.setString(ss.str());

    // Update player GUI
    float hpPercent = static_cast<float>(this->player->getHp()) / this->player->getHpMax();
    this->playerHpBar.setSize(sf::Vector2f(300.f * hpPercent, this->playerHpBar.getSize().y));
}

void Game::updateWorld()
{
    if (bgY < 600)
        bgY -= 1.f * .1;
    else
        bgY = 0;

    this->worldBackground.setTextureRect(sf::IntRect(0, bgY, 800, 600));
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
    // Spawning
    this->spawnTimer += 0.5f;
    if (this->spawnTimer >= this->spawnTimerMax)
    {
        this->enemies.push_back(new Enemy(rand() % this->window->getSize().x-20.f, -100.f));
        this->spawnTimer = 0.f;
    }

    //update
    unsigned counter = 0;
    for (auto *enemy : this->enemies)
    {
        enemy->update();

        // Enemy culling (when reach bottom of screen)
        if (enemy->getBounds().top > this->window->getSize().y) // when top of frame = bottom of screen
        {
            // Delete current enemy
            delete this->enemies.at(counter);
            this->enemies.erase(this->enemies.begin() + counter);
        }
        // Enemy/Player Collision
        else if (enemy->getBounds().intersects(this->player->getBounds()))
        {
            this->player->loseHp(this->enemies.at(counter)->getDamage());
            delete this->enemies.at(counter);
            this->enemies.erase(this->enemies.begin() + counter);
        }

        ++counter; // when used alone, optimizes
    }
}

void Game::updateCombat()
{
    // for every enemy and every bullet, check if enemie[i] crosses boundary of bullets[k], erase that enemy and that bullet
    for (int i = 0; i < this->enemies.size(); ++i)
    {
        bool enemy_deleted = false;
        for (size_t k = 0; k < this->bullets.size() && enemy_deleted == false; k++)
        {
            if (this->enemies[i]->getBounds().intersects(this->bullets[k]->getBounds()))
            {
                this->points += this->enemies[i]->getPoints();

                delete this->enemies[i];
                this->enemies.erase(this->enemies.begin() + i);

                delete this->bullets[k];
                this->bullets.erase(this->bullets.begin() + k);

                enemy_deleted = true;
            }
        }
    }
}

void Game::update()
{
    //this->updatePollEvents();

    this->updateInput();

    this->player->update();

    this->updateCollision();

    this->updateBullets();

    this->updateEnemies();

    this->updateCombat();

    this->updateGUI();

    this->updateWorld();
}

void Game::renderGUI()
{
    this->window->draw(this->pointText);
    this->window->draw(this->playerhpBarBack);
    this->window->draw(this->playerHpBar);
}

void Game::updateCollision()
{
    // PLAYER CANNOT MOVE OUT OF SCREEN
    if (this->player->getBounds().left < 0.f)
    {
        this->player->setPosition(0.f, this->player->getBounds().top);
    }
    else if (this->player->getBounds().left >= this->window->getSize().x - this->player->getBounds().width)
    {
        this->player->setPosition(this->window->getSize().x - this->player->getBounds().width, this->player->getBounds().top);
    }
    if (this->player->getBounds().top < 0.f)
    {
        this->player->setPosition(this->player->getBounds().left, 0.f);
    }
    else if (this->player->getBounds().top >= this->window->getSize().y - this->player->getBounds().height)
    {
        this->player->setPosition(this->player->getBounds().left, this->window->getSize().y - this->player->getBounds().height);
    }
}

void Game::renderWorld()
{
    this->window->draw(this->worldBackground);
}

void Game::render()
{
    this->window->clear();

    // Draw world elements (background)
    this->renderWorld();

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

    this->renderGUI();

    //Game Over Screen
    if (this->player->getHp() <= 0)
        this->window->draw(this->gameOverText);

    this->window->display();
}