#ifndef PLAYER_H
#define PLAYER_H

#include "../AnimationManager/AnimationManager.hpp"
#include "../Level/Level.h"

using namespace std;

class Player
{
  public:
    float x, y, dx, dy, width, height, moveCounter;
    bool attack, flip, onGround, onJump, canMove, keyPressed;
    sf::Vector2f position;
    vector<Object> groundObjects;
    Object mainGroundObject;

    std::map<std::string, bool> key;

    Player(AnimationManager &animationManager, Texture &jediTexture, vector<Object> GroundObjects)
    {
        initObjects(GroundObjects);
        position = {100, (float)mainGroundObject.rect.top};
        attack = onGround = onJump = keyPressed = false;
        canMove = true;
        animationManager.setPosition(position);
        if (!jediTexture.loadFromFile(AnimConfig::JEDI_TEXTURE_PATH))
        {
            std::exit(0);
        }
        animationManager.loadFromXml("textures/PlayerAnimations.xml", jediTexture);
    }

    void update(float time, AnimationManager &animationManager)
    {
        keyPressed = false;
        onGround = position.y >= (float)mainGroundObject.rect.top;
        moveHandler(time, animationManager);
        blockHandler(time, animationManager);
        jumpHandler(time, animationManager);
        forceHandler(time, animationManager);
        attackHandler(time, animationManager);
        canMove = !keyPressed;

        animationManager.flip(flip);
        animationManager.setPosition(position);
        animationManager.tick(time);
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

    void moveHandler(float time, AnimationManager &animationManager)
    {
        if (!onGround && !onJump)
        {
            position.y += 0.15 * time;
        }
        if (Keyboard::isKeyPressed(Keyboard::Right) && canMove)
        {
            if (onGround)
            {
                animationManager.set(AnimConfig::WALK_ANIMATION);
                flip = false;
            }

            position.x += 0.5 * time;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Left) && canMove)
        {
            if (onGround)
            {
                animationManager.set(AnimConfig::WALK_ANIMATION);
                flip = true;
            }
            position.x -= 0.5 * time;
        }
        else if (onGround)
        {
            animationManager.set(AnimConfig::STAY_ANIMATION);
        }
    }

    void blockHandler(float time, AnimationManager &animationManager)
    {
        if (Keyboard::isKeyPressed(Keyboard::R) && onGround)
        {
            animationManager.set(AnimConfig::BLOCK_ANIMATION);
            canMove = false;
            keyPressed = true;
        }
    }

    void jumpHandler(float time, AnimationManager &animationManager)
    {
        if (Keyboard::isKeyPressed(Keyboard::Up) && onGround && canMove)
        {
            animationManager.set(AnimConfig::JUMP_ANIMATION);
            animationManager.resetAnimation();
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

    void forceHandler(float time, AnimationManager &animationManager)
    {
        if (Keyboard::isKeyPressed(Keyboard::Q) && onGround)
        {
            animationManager.set(AnimConfig::FORCE_ANIMATION);
            canMove = false;
            keyPressed = true;
        }
    }

    void attackHandler(float time, AnimationManager &animationManager)
    {
        if (Keyboard::isKeyPressed(Keyboard::Space) && onGround)
        {
            animationManager.set(AnimConfig::ATTACK_ANIMATION);
            canMove = false;
            keyPressed = true;
        }
    }
};

#endif