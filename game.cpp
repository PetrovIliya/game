#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <iostream>
#include <cassert>
#include "src/GameManager/GameManager.h"

int main()
{
    sf::Event event;
    sf::RenderWindow window(sf::VideoMode({WindowConfig::WINDOW_WIDTH, WindowConfig::WINDOW_HEIGHT}), "Star wars");
    sf::View view;
    view.setCenter(WindowConfig::WINDOW_WIDTH / 2, WindowConfig::WINDOW_HEIGHT / 2);
    view.setSize(WindowConfig::WINDOW_WIDTH, WindowConfig::WINDOW_HEIGHT);
    GameManager gameManager;

    while (window.isOpen())
    {
        window.setView(view);
        gameManager.update(window, view);
        window.display();
    }
    return 0;
}