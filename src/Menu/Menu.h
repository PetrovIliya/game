#ifndef GAME_MENU_H
#define GAME_MENU_H

#include <SFML/System.hpp>
#include "../Screen/Screen.h"
#include <SFML/Graphics.hpp>
#include <iostream>

class Menu : public Screen
{
  public:
    sf::Texture background, newGame, continueT, exitTexture, playerDeathTexture;
    sf::Sprite backgroundSprite, newGameSprite, continueSprite, exitSprite, playerDeathSprite;
    std::vector<sf::Sprite> menuItems;
    bool movingDown, movingUp;
    int activeSpriteIndex;

    Menu();

    void update(sf::RenderWindow &window, sf::View &view, bool isPlayerAlive);

  private:
    void setCurrentPosition(int leftEndOfView);

    void draw(sf::RenderWindow &window, bool isPlayerAlive);

    void selectMenuItemHandler();

    Player getPlayer();
};

#endif
