#include "Clone.h"

void Clone::draw(sf::RenderWindow &window, float time)
{
    animationManager.flip(flip);
    animationManager.setPosition(position);
    animationManager.tick(time);
    animationManager.draw(window);
}

void Clone::setCurrentAnimation(sf::String name)
{
    animationManager.set(name);
}

AnimationManager *Clone::getAnimationManager()
{
    return &animationManager;
}

void Clone::update(float elapsedTime, sf::Clock &elapsedClock, float deltaTime, int playerPosition)
{
    moveHandler(playerPosition, deltaTime);
    animationManager.setPosition(position);
    attackHandler();
    lifeHandler();
    stasisHandler(elapsedTime, elapsedClock);
}

int Clone::getCurrentSpriteSize()
{
    return animationManager.getCurrentSpriteSize();
}

int Clone::getCurrentSpriteHeight()
{
    return animationManager.getCurrentSpriteHeight();
}

bool Clone::isMoving()
{
    return animationManager.getCurrentAnimationName() == AnimConfig::WALK_ANIMATION;
}

void Clone::moveHandler(float playerPosition, float time)
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

void Clone::attackHandler()
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

void Clone::lifeHandler()
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

void Clone::stasisHandler(float elapsedTime, sf::Clock &elapsedClock)
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

void Clone::setAnimation(sf::String name)
{
    animationManager.set(name);
}

bool Clone::isFlalling()
{
    return animationManager.getCurrentAnimationName() == AnimConfig::FALL_ANIMATION;
}