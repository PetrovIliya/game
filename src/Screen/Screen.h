#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

class Screen
{
public:
    bool needToReset = false;
    virtual void update(sf::RenderWindow &window, sf::View &view) = 0;
};

#endif
