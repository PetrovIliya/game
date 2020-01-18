#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "../Player/Player.h"

class Screen
{
  public:
    bool needToReset = false;
    virtual Player getPlayer() = 0;
    virtual void update(sf::RenderWindow &window, sf::View &view, bool isPlayerAlive) = 0;
};

#endif
