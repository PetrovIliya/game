#ifndef GAME_CLONE_H
#define GAME_CLONE_H

#include "../Enemy/Enemy.h"
#include "../AnimationManager/AnimationManager.hpp"

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

    void update(float time)
    {
        enemyLogic();
    }

  private:
    void enemyLogic()
    {
        if (isAttack && !isWounded && !isTraped)
        {
            setAnimation(AnimConfig::ATTACK_ANIMATION);
        }
        else if (!isWounded && !isTraped)
        {
            setAnimation(AnimConfig::STAY_ANIMATION);
        }
        if (isWounded && !isFlalling() && !isTraped)
        {
            setAnimation(AnimConfig::FALL_ANIMATION);
        }
        else if (isFlalling() && animationManager.isLastFrame())
        {
            animationManager.pause();
        }

        if (isTraped && animationManager.isLastFrame())
        {
            animationManager.pause();
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
