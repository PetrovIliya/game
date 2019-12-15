#ifndef GAME_ENEMY_H
#define GAME_ENEMY_H

#include "../AnimationManager/AnimationManager.hpp"
#include "../Level/Level.h"

class Enemy
{
  public:
    bool isAttack, flip, onGround, onJump, canMove, isWounded, isTraped;
    sf::Vector2f position;
    vector<Object> groundObjects;
    Object mainGroundObject;

    Enemy(vector<Object> GroundObjects, sf::Vector2f Position)
    {
        initObjects(GroundObjects);
        position = Position;
        isAttack = onGround = onJump = isWounded = isTraped = false;
        canMove, flip = true;
    }

    virtual void draw(sf::RenderWindow &window, float time) = 0;

    virtual void update(float time) = 0;

    virtual AnimationManager *getAnimationManager() = 0;

    virtual void setCurrentAnimation(sf::String name) = 0;

    void setIsAttack(bool isAttack)
    {
        Enemy::isAttack = isAttack;
    }

    sf::Vector2f getPosition()
    {
        return position;
    }

    void initObjects(vector<Object> GroundObjects)
    {
        groundObjects = GroundObjects;
        for (int i = 0; i < groundObjects.size(); i++)
        {
            if (groundObjects[i].type == "main")
            {
                mainGroundObject = groundObjects[i];
                break;
            }
        }
    }

  private:
};

#endif
