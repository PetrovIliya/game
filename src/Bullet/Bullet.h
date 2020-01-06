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
    bool isAlive, isReflected;
    float bulletSpeed;
    Vector2f position;

    Bullet(sf::Vector2f position, bool enemyFlip);

    void update(float time, RenderWindow &window, sf::View &view);

    void changeDirection();

    void draw(RenderWindow &window);
};
#endif
