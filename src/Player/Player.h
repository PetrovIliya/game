#ifndef PLAYER_H
#define PLAYER_H

#include "../AnimationManager/AnimationManager.hpp"
#include "../Level/Level.h"
#include "../../config/WindowConfig.hpp"

using namespace std;

class Player
{
  public:
    AnimationManager animationManager;
    bool attack, flip, onGround, onJump, canMove, keyPressed, underAttack, isAlive;
    sf::Vector2f position;
    vector<Object> groundObjects;
    Object mainGroundObject;

    Player(Texture &jediTexture, vector<Object> GroundObjects)
    {
        initObjects(GroundObjects);
        position = {100, (float)mainGroundObject.rect.top};
        attack = onGround = onJump = keyPressed = false;
        canMove = isAlive = true;
        setPosition(position);
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
        keyPressed = false;
        onGround = position.y >= (float)mainGroundObject.rect.top;
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
    }

    void setView(sf::View &view, int realWindowWidth, int tileWidth)
    {
        float centerX = WindowConfig::WINDOW_WIDTH / 2;
        if (position.x > centerX && position.x < realWindowWidth - centerX)
        {
            view.setCenter(position.x, WindowConfig::WINDOW_HEIGHT / 2);
        }
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
        if (Keyboard::isKeyPressed(Keyboard::Right) && ableToMove())
        {
            setWalkAnimationAndDirection(false);
            position.x += 0.5 * time;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Left) && ableToMove())
        {
            setWalkAnimationAndDirection(true);
            position.x -= 0.5 * time;
        }
        else if (onGround && !underAttack && isAlive)
        {
            setStayAnimation();
        }
    }

    bool ableToMove()
    {
        return canMove && !isReflecting() && !isBlocking() && isAlive;
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
        if (Keyboard::isKeyPressed(Keyboard::Q) && onGround && isAlive)
        {
            setFroceAnimation();
            canMove = false;
            keyPressed = true;
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

    void setPosition(Vector2f position)
    {
        animationManager.setPosition(position);
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

    bool isReflecting()
    {
        return getCurrentAnimName() == AnimConfig::REFLECT_ANIMATION;
    }

    bool isBlocking()
    {
        return getCurrentAnimName() == AnimConfig::BLOCK_ANIMATION;
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