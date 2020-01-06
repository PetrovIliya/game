#ifndef GAME_GAME_H
#define GAME_GAME_H

#include "../Screen/Screen.h"
#include "../Menu/Menu.h"
#include "../GameManager/GameManager.h"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <iostream>

class Game
{
public:
    Game();

    void start(sf::RenderWindow &window, sf::View &view);

private:
    sf::Event event;
    Menu menu;
    std::string currentScreen;
    std::map<std::string, Screen*> screens;
    bool enter;

    void pollEvents(sf::RenderWindow &window);

    void choseHandler(sf::RenderWindow &window, sf::View &view);

    void menuHandler();

    void screenHandler(sf::RenderWindow &window);
};

#endif
