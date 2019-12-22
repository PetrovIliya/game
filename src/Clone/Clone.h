#ifndef GAME_CLONE_H
#define GAME_CLONE_H

#include "../Enemy/Enemy.h"
#include "../AnimationManager/AnimationManager.hpp"
#include "../../config/EnemyConfig.cpp"

class Clone : public Enemy
{
  public:
    AnimationManager animationManager;

    Clone(Texture &cloneTexture, vector<Object> GroundObjects, sf::Vector2f Position) : Enemy::Enemy(GroundObjects, Position)
    {
        if (!cloneTexture.loadFromFile(AnimConfig::CLONE_TEXTURE_PATH))
        {
            std::exit(0);
        }
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

    void update(float elapsedTime, sf::Clock &elapsedClock)
    {
        attackAndLifeHandler();
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

  private:
    void attackAndLifeHandler()
    {
        if (isAttack && !isWounded && !isTraped)
        {
            setAnimation(AnimConfig::ATTACK_ANIMATION);
        }
        else if (!isWounded && !isTraped)
        {
            setAnimation(AnimConfig::STAY_ANIMATION);
        }
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
            if (animationManager.isLastFrame())
            {
                animationManager.pause();
            }
            if (elapsedTime >= EnemyConfig::STASIS_TIME)
            {
                isTraped = false;
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
