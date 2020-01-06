#ifndef GAME_CLONE_H
#define GAME_CLONE_H

#include "../Enemy/Enemy.h"
#include "../AnimationManager/AnimationManager.hpp"
#include "../../config/EnemyConfig.cpp"

class Clone : public Enemy
{
  public:
    AnimationManager animationManager;

    Clone(Texture &cloneTexture, sf::Vector2f Position, int id, bool flip) : Enemy::Enemy(Position, id)
    {
        if (!cloneTexture.loadFromFile(AnimConfig::CLONE_TEXTURE_PATH))
        {
            std::exit(0);
        }
        Enemy::flip = flip;
        animationManager.loadFromXml(AnimConfig::CLONE_ANIMS_PATH, cloneTexture);
        animationManager.setPosition(position);
        animationManager.set(AnimConfig::STAY_ANIMATION);
    }

    void draw(sf::RenderWindow &window, float time);

    void setCurrentAnimation(sf::String name);

    AnimationManager *getAnimationManager();

    void update(float elapsedTime, sf::Clock &elapsedClock, float deltaTime, int playerPosition);

    int getCurrentSpriteSize();

    int getCurrentSpriteHeight();

    bool isMoving();

  private:
    void moveHandler(float playerPosition, float time);

    void attackHandler();

    void lifeHandler();

    void stasisHandler(float elapsedTime, sf::Clock &elapsedClock);

    void setAnimation(sf::String name);

    bool isFlalling();
};

#endif
