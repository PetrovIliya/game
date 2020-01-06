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

    void draw(sf::RenderWindow &window, float time)
    {
        animationManager.flip(flip);
        animationManager.setPosition(position);
        animationManager.tick(time);
        animationManager.draw(window);
    }

    void setCurrentAnimation(sf::String name)
    {
        animationManager.set(name);
    }

    AnimationManager *getAnimationManager()
    {
        return &animationManager;
    }

    void update(float elapsedTime, sf::Clock &elapsedClock, float deltaTime, int playerPosition)
    {
        moveHandler(playerPosition, deltaTime);
        animationManager.setPosition(position);
        attackHandler();
        lifeHandler();
        stasisHandler(elapsedTime, elapsedClock);
    }

    int getCurrentSpriteSize()
    {
        return animationManager.getCurrentSpriteSize();
    }

    int getCurrentSpriteHeight()
    {
        return animationManager.getCurrentSpriteHeight();
    }

    bool isMoving()
    {
        return animationManager.getCurrentAnimationName() == AnimConfig::WALK_ANIMATION;
    }

  private:
    void moveHandler(float playerPosition, float time)
    {
        if (playerPosition < position.x && !isWounded && ableToMoveLeft)
        {
            if (position.x - playerPosition > EnemyConfig::VIEW_DISTANCE && !isTraped)
            {
                animationManager.set(AnimConfig::WALK_ANIMATION);
                position.x -= time * 0.3;
            }
        }
        else if (!isWounded && ableToMoveRight)
        {
            if (playerPosition - position.x > EnemyConfig::VIEW_DISTANCE && !isTraped)
            {
                animationManager.set(AnimConfig::WALK_ANIMATION);
                position.x += time * 0.3;
            }
        }
    }

    void attackHandler()
    {
        if (isAttack && !isWounded && !isTraped)
        {
            setAnimation(AnimConfig::ATTACK_ANIMATION);
        }
        else if (!isWounded && !isTraped && !isMoving())
        {
            setAnimation(AnimConfig::STAY_ANIMATION);
        }
    }

    void lifeHandler()
    {
        if (isWounded && !isFlalling())
        {
            setAnimation(AnimConfig::FALL_ANIMATION);
        }
        else if (isFlalling() && animationManager.isLastFrame())
        {
            animationManager.pause();
        }
    }

    void stasisHandler(float elapsedTime, sf::Clock &elapsedClock)
    {
        if (isTraped)
        {
            if (needRestart)
            {
                elapsedClock.restart();
                needRestart = false;
            }
            if (animationManager.isLastFrame())
            {
                animationManager.pause();
            }
            if (elapsedTime >= EnemyConfig::STASIS_TIME)
            {
                isTraped = false;
                needRestart = true;
                elapsedClock.restart();
            }
        }
    }

    void setAnimation(sf::String name)
    {
        animationManager.set(name);
    }

    bool isFlalling()
    {
        return animationManager.getCurrentAnimationName() == AnimConfig::FALL_ANIMATION;
    }
};

#endif
