#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <iostream>
#include <cassert>
#include "src/GameManager/GameManager.h"

void pollEvents(sf::RenderWindow &window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            window.close();
            break;
        default:
            break;
        }
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({WindowConfig::WINDOW_WIDTH, WindowConfig::WINDOW_HEIGHT}), "Star wars");
    sf::View view;
    view.setCenter(WindowConfig::WINDOW_WIDTH / 2, WindowConfig::WINDOW_HEIGHT / 2);
    view.setSize(WindowConfig::WINDOW_WIDTH, WindowConfig::WINDOW_HEIGHT);
    GameManager gameManager;

    while (window.isOpen())
    {
        pollEvents(window);
        window.clear(Color::White);
        window.setView(view);
        gameManager.update(window, view);
        window.display();
    }
    return 0;
}