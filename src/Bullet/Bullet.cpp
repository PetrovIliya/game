#include "Bullet.h"

Bullet::Bullet(sf::Vector2f position, bool enemyFlip)
{
    isReflected = false;
    bulletSpeed = 0.2f;
    Bullet::position = position;
    if (enemyFlip)
    {
        bulletSpeed = -bulletSpeed;
    }
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

void Bullet::update(float time, RenderWindow &window, sf::View &view)
{
    Vector2f pos = bullet.getPosition();
    int leftEndOfView = view.getCenter().x - (window.getSize().x / 2);
    int rightEndOfView = view.getCenter().x + (window.getSize().x / 2);

    if (pos.x < leftEndOfView || pos.x > rightEndOfView)
    {
        isAlive = false;
    }

    pos.x += bulletSpeed * time;
    bullet.setPosition(pos);
    Bullet::position = pos;
}

void Bullet::changeDirection()
{
    isReflected = true;
    bulletSpeed = -bulletSpeed;
}

void Bullet::draw(RenderWindow &window)
{
    window.draw(bullet);
}
