#ifndef PLAYER_H
#define PLAYER_H

#include "../AnimationManager/AnimationManager.hpp"
#include "../Level/Level.h"
#include "../../config/WindowConfig.hpp"
#include "../../config/PlayerConfig.cpp"

using namespace std;

class Player
{
  public:
    AnimationManager animationManager;
    bool attack, flip, onGround, onJump, canMove, keyPressed, underAttack, isAlive, onCoolDown, canUseForce, ableToMoveRight, ableToMoveLeft;
    sf::Vector2f position, lastPosition;
    vector<Object> groundObjects;
    Object mainGroundObject;
    sf::Clock elapsedClock;
    float elapsedTime;

    Player(Texture &jediTexture, vector<Object> GroundObjects)
    {
        initObjects(GroundObjects);
        position = {100, (float)mainGroundObject.rect.top};
        attack = onGround = onJump = keyPressed = onCoolDown = false;
        canMove = isAlive = canUseForce = ableToMoveRight = ableToMoveLeft = true;
        setPosition(position);
        lastPosition = {0, 0};
        if (!jediTexture.loadFromFile(AnimConfig::JEDI_TEXTURE_PATH))
        {
            std::exit(0);
        }
        animationManager.loadFromXml(AnimConfig::JEDI_ANIMS_PATH, jediTexture);
    }

    Player()
    {
    }

    void update(float time, sf::RenderWindow &window)
    {
        elapsedTime = elapsedClock.getElapsedTime().asSeconds();
        keyPressed = false;
        onGround = position.y >= (float)mainGroundObject.rect.top;
        coolDowndHandler();
        lifeHandler();
        moveHandler(time);
        jumpHandler(time);
        reflectHandler();
        blockHandler();
        forceHandler();
        attackHandler();
        canMove = !keyPressed;
        animationManager.flip(flip);
        animationManager.setPosition(position);
        animationManager.tick(time);
        animationManager.draw(window);
        lastPosition = position;
    }

    void setView(sf::View &view, int realWindowWidth, int tileWidth)
    {
        float centerX = WindowConfig::WINDOW_WIDTH / 2;
        if (position.x > centerX && position.x < realWindowWidth - centerX)
        {
            view.setCenter(position.x, WindowConfig::WINDOW_HEIGHT / 2);
        }
    }

    void restartTime()
    {
        elapsedClock.restart();
    }

    void coolDowndHandler()
    {
        if (onCoolDown && elapsedTime >= PlayerConfig::STASIS_COOLDOWN)
        {
            onCoolDown = false;
            canUseForce = true;
        }
    }

    bool ableToMove()
    {
        return canMove && !isReflecting() && !isBlocking() && isAlive;
    }

    void setPosition(Vector2f position)
    {
        animationManager.setPosition(position);
    }

    bool isReflecting()
    {
        return getCurrentAnimName() == AnimConfig::REFLECT_ANIMATION;
    }

    bool isBlocking()
    {
        return getCurrentAnimName() == AnimConfig::BLOCK_ANIMATION;
    }

    bool onForce()
    {
        return getCurrentAnimName() == AnimConfig::FORCE_ANIMATION;
    }

    bool isAttack()
    {
        return getCurrentAnimName() == AnimConfig::ATTACK_ANIMATION;
    }

    int getCurrentSpriteSize()
    {
        animationManager.getCurrentSpriteSize();
    }

  private:
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

    void lifeHandler()
    {
        if (!isAlive)
        {
            setFallAnimation();
        }
    }

    void moveHandler(float time)
    {
        if (!onGround && !onJump)
        {
            position.y += 0.15 * time;
        }
        if (Keyboard::isKeyPressed(Keyboard::Right) && ableToMove() && ableToMoveRight)
        {
            setWalkAnimationAndDirection(false);
            position.x += 0.5 * time;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Left) && ableToMove() && ableToMoveLeft)
        {
            setWalkAnimationAndDirection(true);
            position.x -= 0.5 * time;
        }
        else if (onGround && !underAttack && isAlive)
        {
            setStayAnimation();
        }
    }

    void reflectHandler()
    {
        if (underAttack && isBlocking())
        {
            setReflectAnimation();
        }
        else if (!isReflecting() && !Keyboard::isKeyPressed(Keyboard::R))
        {
            underAttack = false;
        }
        if (isReflecting() && (animationManager.isLastFrame() || !Keyboard::isKeyPressed(Keyboard::R)))
        {
            resetAnimation();
            setStayAnimation();
        }
    }

    void blockHandler()
    {
        if (Keyboard::isKeyPressed(Keyboard::R) && onGround && !isReflecting() && isAlive)
        {
            setBlockAnimation();
            canMove = false;
            keyPressed = true;
        }
    }

    void jumpHandler(float time)
    {
        if (Keyboard::isKeyPressed(Keyboard::Up) && onGround && canMove && isAlive)
        {
            setJumpAnimation();
            resetAnimation();
            onJump = true;
            keyPressed = true;
        }

        if (onJump)
        {
            position.y -= 0.15 * time;
            float deltaJumpHeight = (float)mainGroundObject.rect.top - position.y;
            if (deltaJumpHeight >= AnimConfig::JUMP_HEIGHT)
            {
                onJump = false;
            }
        }
    }

    void forceHandler()
    {
        if (Keyboard::isKeyPressed(Keyboard::Q) && onGround && isAlive && canUseForce)
        {
            if (!onCoolDown)
            {
                restartTime();
            }
            onCoolDown = true;
            setFroceAnimation();
            canMove = false;
            keyPressed = true;
        }
        if (onCoolDown && !Keyboard::isKeyPressed(Keyboard::Q))
        {
            canUseForce = false;
        }
    }

    void attackHandler()
    {
        if (Keyboard::isKeyPressed(Keyboard::Space) && onGround && isAlive)
        {
            setAttackAnimation();
            canMove = false;
            keyPressed = true;
        }
    }

    void setWalkAnimation()
    {
        animationManager.set(AnimConfig::WALK_ANIMATION);
    }

    void setStayAnimation()
    {
        animationManager.set(AnimConfig::STAY_ANIMATION);
    }

    void setBlockAnimation()
    {
        animationManager.set(AnimConfig::BLOCK_ANIMATION);
    }

    void setReflectAnimation()
    {
        animationManager.set(AnimConfig::REFLECT_ANIMATION);
    }

    void setAttackAnimation()
    {
        animationManager.set(AnimConfig::ATTACK_ANIMATION);
    }

    void setFroceAnimation()
    {
        animationManager.set(AnimConfig::FORCE_ANIMATION);
    }

    void setJumpAnimation()
    {
        animationManager.set(AnimConfig::JUMP_ANIMATION);
    }

    void setFallAnimation()
    {
        animationManager.set(AnimConfig::FALL_ANIMATION);
    }

    sf::String getCurrentAnimName()
    {
        return animationManager.getCurrentAnimationName();
    }

    void resetAnimation()
    {
        animationManager.resetAnimation();
    }

    void setWalkAnimationAndDirection(bool flip)
    {
        if (onGround)
        {
            setWalkAnimation();
            Player::flip = flip;
        }
    }
};

#endif