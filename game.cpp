#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <vector>
#include <iostream>
#include <random>
#include <cassert>
#include <ctime>
#include "src/AnimationManager/AnimationManager.hpp"
#include "config/AnimConfig.cpp"
#include "config/WindowConfig.hpp"
#include "src/Player/Player.h"
#include "src/Level/Level.h"

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
    sf::Clock deltaClock;
    Level level;
    level.LoadFromFile("maps/dantuinMap.tmx");
    AnimationManager animationManager;
    int realWindowWidth = level.GetWindowWidth();
    Texture jediTexture;
    Player player(animationManager, jediTexture, level.GetObjects("ground"));
    while (window.isOpen())
    {
        float deltaTime = deltaClock.restart().asMicroseconds();
        deltaTime = deltaTime / 1000;
        pollEvents(window);
        player.setView(view, realWindowWidth, level.tileWidth);
        player.update(deltaTime, animationManager);
        window.clear(Color::White);
        window.setView(view);
        level.Draw(window);
        animationManager.draw(window);
        window.display();
    }
    return 0;
}