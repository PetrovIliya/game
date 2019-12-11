#ifndef GAME_BULLETS_H
#define GAME_BULLETS_H

#include "../Bullet/Bullet.h"
#include <list>

class Bullets
{
  public:
    std::list<Bullet *> bullets;
    std::list<Bullet *>::iterator it;

    void add(Vector2f position)
    {
        bullets.push_back(new Bullet(position));
    }

    void update(float time, RenderWindow &window)
    {
        for (it = bullets.begin(); it != bullets.end(); it++)
        {
            Bullet *b = *it;
            (*it)->update(time);
            (*it)->draw(window);
            if (!b->isAlive)
            {
                it = bullets.erase(it);
                delete b;
                std::cout << "x " << std::endl;
            }
        }
    }
};

#endif
