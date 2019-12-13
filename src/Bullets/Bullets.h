#ifndef GAME_BULLETS_H
#define GAME_BULLETS_H

#include "../Bullet/Bullet.h"
#include "../Player/Player.h"
#include <list>

class Bullets
{
  public:
    std::list<Bullet *> bullets;
    std::list<Bullet *>::iterator it;

    void add(Vector2f position, bool isEnemyFlip)
    {
        if (!isEnemyFlip)
        {
            position.x += 10;
        }
        position.y -= 38;
        bullets.push_back(new Bullet(position, isEnemyFlip));
    }

    void update(float time, RenderWindow &window, sf::View &view, Player &player)
    {
        int spriteSize = fabs(player.animationManager.getCurrentSpriteSize());
        for (it = bullets.begin(); it != bullets.end(); it++)
        {
            Bullet *b = *it;
            (*it)->update(time, window, view);
            (*it)->draw(window);
            if ((*it)->position.x >= player.position.x && (*it)->position.x <= player.position.x + spriteSize)
            {
                if (player.underAttack && player.isReflecting())
                {
                    (*it)->bulletSpeed = -(*it)->bulletSpeed;
                }
                player.underAttack = true;
            }
            if (!b->isAlive)
            {
                it = bullets.erase(it);
                delete b;
            }
        }
    }
};

#endif
