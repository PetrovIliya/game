#ifndef GAME_ENEMY_H
#define GAME_ENEMY_H

#include "../AnimationManager/AnimationManager.hpp"
#include "../Level/Level.h"

class Enemy
{
  public:
    bool isAttack, flip, onGround, onJump, canMove, isWounded, isTraped, isFlalling, onColission, ableToMoveRight, ableToMoveLeft, needRestart;
    sf::Vector2f position;
    int id;

    Enemy(sf::Vector2f Position, int id)
    {
        Enemy::id = id;
        position = Position;
        isAttack = onGround = onJump = isWounded = isTraped = isFlalling = onColission = false;
        canMove = flip = ableToMoveRight = ableToMoveLeft = needRestart = true;
    }

    virtual void draw(sf::RenderWindow &window, float time) = 0;

    virtual void update(float elapsedTime, sf::Clock &elapsedClock, float deltaTime, int playerPosition) = 0;

    virtual AnimationManager *getAnimationManager() = 0;

    virtual void setCurrentAnimation(sf::String name) = 0;

    virtual int getCurrentSpriteSize() = 0;

    virtual int getCurrentSpriteHeight() = 0;

    virtual bool isMoving() = 0;

    void setIsAttack(bool isAttack)
    {
        Enemy::isAttack = isAttack;
    }

    sf::Vector2f getPosition()
    {
        return position;
    }

  private:
};

#endif
