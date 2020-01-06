#ifndef GAME_MENU_H
#define GAME_MENU_H

#include <SFML/System.hpp>
#include "../Screen/Screen.h"
#include <SFML/Graphics.hpp>
#include <iostream>

class Menu : public Screen
{
public:
    sf::Texture background, newGame, continueT, exitTexture;
    sf::Sprite backgroundSprite, newGameSprite, continueSprite, exitSprite;
    std::vector<sf::Sprite> menuItems;
    bool movingDown, movingUp;
    int activeSpriteIndex;

    Menu()
    {
        activeSpriteIndex = 0;
        movingDown = movingUp = false;
        background.loadFromFile("textures/revan.jpg");
        newGame.loadFromFile("textures/newGame.png");
        exitTexture.loadFromFile("textures/exit.png");
        continueT.loadFromFile("textures/continueActive.png");
        backgroundSprite.setTexture(background);
        newGameSprite.setTexture(newGame);
        exitSprite.setTexture(exitTexture);
        continueSprite.setTexture(continueT);
        backgroundSprite.setPosition({0, 0});
        newGameSprite.setPosition({50, 400});
        continueSprite.setPosition({50, 300});
        exitSprite.setPosition({50, 500});
        menuItems.push_back(continueSprite);
        menuItems.push_back(newGameSprite);
        menuItems.push_back(exitSprite);
    }

    void update(sf::RenderWindow &window, sf::View &view)
    {
        int leftEndOfView = view.getCenter().x - (window.getSize().x / 2);
        setCurrentPosition(leftEndOfView);
        selectMenuItemHandler();
        draw(window);
    }

    void setCurrentPosition(int leftEndOfView)
    {
        backgroundSprite.setPosition({leftEndOfView, 0});
        menuItems[0].setPosition({leftEndOfView + 50, 300});
        menuItems[1].setPosition({leftEndOfView + 50, 400});
        menuItems[2].setPosition({leftEndOfView + 50, 500});

    }

    void draw(sf::RenderWindow &window)
    {
        window.draw(backgroundSprite);
        for (int i = 0; i < menuItems.size(); i++)
        {
            window.draw(menuItems[i]);
        }
    }

    void selectMenuItemHandler()
    {
        if (movingDown)
        {
            movingDown = false;
            if (activeSpriteIndex == 1)
            {
                activeSpriteIndex++;
                newGame.loadFromFile("textures/newGame.png");
                exitTexture.loadFromFile("textures/exitActive.png");
            }
            if (activeSpriteIndex == 0)
            {
                activeSpriteIndex++;
                continueT.loadFromFile("textures/continue.png");
                newGame.loadFromFile("textures/newGameActive.png");
            }
        }
        if(movingUp)
        {
            movingUp = false;
            if (activeSpriteIndex == 1)
            {
                activeSpriteIndex--;
                continueT.loadFromFile("textures/continueActive.png");
                newGame.loadFromFile("textures/newGame.png");
            }
            if (activeSpriteIndex == 2)
            {
                activeSpriteIndex--;
                exitTexture.loadFromFile("textures/exit.png");
                newGame.loadFromFile("textures/newGameActive.png");
            }
        }
    }

};

#endif
