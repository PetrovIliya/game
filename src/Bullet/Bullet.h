#ifndef GAME_BULLET_H
#define GAME_BULLET_H

#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;

class Bullet
{
  public:
    sf::ConvexShape bullet;
    bool isAlive;
    float bulletSpeed;

    Bullet(sf::Vector2f position)
    {
        bulletSpeed = 0.1f;
        isAlive = true;
        sf::ConvexShape bulletT;
        bulletT.setFillColor(sf::Color(255, 0, 0));
        bulletT.setPointCount(30);

        sf::Vector2f bulletRadius = {7.f, 2.f};
        for (int i = 0; i < 30; ++i)
        {
            float angle = float(2 * M_PI * i) / float(30);
            sf::Vector2f point = {
                bulletRadius.x * std::sin(angle),
                bulletRadius.y * std::cos(angle)};
            bulletT.setPoint(i, point);
        }

        bulletT.setPosition(position);
        bullet = bulletT;
    }

    void update(float time)
    {
        Vector2f pos = bullet.getPosition();

        if (pos.x < 0 || pos.x > 800)
        {
            isAlive = false;
        }

        pos.x += bulletSpeed * time;
        bullet.setPosition(pos);
    }
    void draw(RenderWindow &window)
    {
        window.draw(bullet);
    }
};
#endif
