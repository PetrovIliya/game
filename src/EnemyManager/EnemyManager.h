#ifndef GAME_ENEMYMANAGER_H
#define GAME_ENEMYMANAGER_H

#include <SFML\Graphics.hpp>
#include <cmath>
#include <list>
#include "../Enemy/Enemy.h"
#include "../Clone/Clone.h"
#include "../../tinyxml/tinyxml.h"
#include "../../config/EnemyConfig.cpp"
#include "../Bullets/Bullets.h"

class EnemyManager
{
  public:
    std::list<Enemy *> enemies;
    std::list<Enemy *>::iterator it;
    float currentEnemyFrame;

    void addClone(Texture &cloneTexture, vector<Object> GroundObjects, sf::Vector2f position)
    {
        enemies.push_back(new Clone(cloneTexture, GroundObjects, position));
    }

    void update(RenderWindow &window, float time, float playerPositionX, Bullets &bullets)
    {
        for (it = enemies.begin(); it != enemies.end(); it++)
        {
            Enemy *e = *it;
            if (!e->isAlive)
            {
                it = enemies.erase(it);
                delete e;
            }
            if (fabs(playerPositionX - (*it)->position.x) <= EnemyConfig::VIEW_DISTANCE)
            {
                (*it)->setIsAttack(true);
                currentEnemyFrame = (*it)->getAnimationManager()->getCurrentFrame();
                if (currentEnemyFrame < 3.1 && currentEnemyFrame > 2.9)
                {
                    Vector2f position = (*it)->position;
                    bullets.add(position, (*it)->flip);
                }
                (*it)->flip = playerPositionX < (*it)->position.x;
            }
            else
            {
                (*it)->setIsAttack(false);
            }
            (*it)->update(time);
            (*it)->draw(window, time);
        }
    }
};

#endif
