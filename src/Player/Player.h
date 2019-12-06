#ifndef PLAYER_H
#define PLAYER_H

#include "../AnimationManager/AnimationManager.hpp"

class Player
{
public:
    float x, y, dx, dy, width, height;
    bool attack, flip;

    enum {stay, walk, jump, death, lightning} state;
    std::map<std::string, bool> key;

    Player(AnimationManager &animationManager)
    {
        attack = false;
        state = stay;

    }

    void update(float time, AnimationManager &animationManager)
    {
        keyCheck();
        if(state == stay)
        {
            animationManager.set("stay");
        }
        if(state == walk)
        {
            animationManager.set(AnimConfig::WALK_ANIMATION);
        }
        if(attack)
        {
            animationManager.set(AnimConfig::ATTACK_ANIMATION);
        }
        if(state == death)
        {
            animationManager.set(AnimConfig::DEATH_NAME);
        }
        if (state == lightning)
        {
            animationManager.set(AnimConfig::LIGHTNING_NAME);
        }
        animationManager.flip(flip);


        x += dx * time;

        dy += 0.0005 * time;
        y += dy * time;
        animationManager.tick(time);
        key["Up"] = key["Space"] = key["R"] = key["L"] = false;
    }

private:
    void keyCheck()
    {
        if (Keyboard::isKeyPressed(Keyboard::Right))
        {
            state = walk;
            flip = false;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Left))
        {
            state = walk;
            flip = true;
        }
        else
        {
            state = stay;
        }
        if (Keyboard::isKeyPressed(Keyboard::Up))
        {
            state = jump;
        }
        if (Keyboard::isKeyPressed(Keyboard::Down))
        {
            state = death;
        }
        if (Keyboard::isKeyPressed(Keyboard::Space))
        {
            attack = true;
        }
        else
        {
            attack = false;
        }
        if ((Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::Left)) && (Keyboard::isKeyPressed(Keyboard::Space)))
        {
            state = lightning;
        }

        //state = stay;

    }
};

#endif

