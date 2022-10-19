#include "headers/Player.h"

// Initializers

void Player::initVariables()
{
    this->movementSpeed = 3.f;

    this->attackCooldownMax = 10.f;
    this->attackCooldown = attackCooldownMax;

    this->hpMax = 100;
    this->hp = this->hpMax;
}

void Player::initTexture()
{
    // Load a texture from file
    if (!this->texture.loadFromFile("textures/ship.png"))
    {
        std::cout << "ERROR: PLAYER : INIT TEXTURE : Could not load texture file." << "\n";
    }
}

void Player::initSprite()
{
    // Set the texture to sprite
    this->sprite.setTexture(this->texture); // if using rectangle shape, &this->texture

    // Resize the sprite
    this->sprite.scale(0.1f, 0.1f); // factor of original
}


// Con/Des
Player::Player()
{
    this->initVariables();
    this->initTexture();
    this->initSprite();
}

Player::~Player()
{
    
}


// Accessors

const sf::Vector2f& Player::getPos() const
{
    return this->sprite.getPosition();
}

const sf::FloatRect Player::getBounds() const
{
    return this->sprite.getGlobalBounds();
}

void Player::move(const float dirX, const float dirY)
{
    this->sprite.move(this->movementSpeed * dirX, this->movementSpeed * dirY);
}

const int& Player::getHp() const
{
    return this->hp;
}

const int& Player::getHpMax() const
{
    return this->hpMax;
}



// Modifiers
void Player::setPosition(const sf::Vector2f pos)
{
    this->sprite.setPosition(pos);
}

void Player::setPosition(const float x, const float y)
{
    this->sprite.setPosition(x, y);
}

void Player::setHp(const int hp)
{
    this->hp = hp;
}

void Player::loseHp(const int value)
{
    this->hp -= value;

    if (this->hp < 0)
        this->hp = 0;
}


//Functions

const bool Player::canAttack()
{
    if (this->attackCooldown >= this->attackCooldownMax)
    {
        this->attackCooldown = 0.f;
        return true;
    }
    return false;
}

void Player::updateAttack()
{
    if (this->attackCooldown < this->attackCooldownMax)
        this->attackCooldown += 0.5f;
}

void Player::update()
{
    this->updateAttack();
}

void Player::render(sf::RenderTarget& target)
{
    target.draw(this->sprite);
}