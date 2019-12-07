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
#include "src/Player/Player.h"
#include "src/Level/Level.h"

constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;

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
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Star wars");
    sf::Clock deltaClock;
    Level level;
    level.LoadFromFile("maps/dantuinMap.tmx");
    AnimationManager animationManager;
    Texture jediTexture;
    Player player(animationManager);
    if (!jediTexture.loadFromFile(AnimConfig::JEDI_TEXTURE_PATH))
    {
        std::exit(0);
    }
    animationManager.loadFromXml("textures/PlayerAnimations.xml", jediTexture);
    while (window.isOpen())
    {
        float deltaTime = deltaClock.restart().asMicroseconds();
        deltaTime = deltaTime / 1000;
        pollEvents(window);

        player.update(deltaTime, animationManager);
        window.clear(Color::White);
        level.Draw(window);
        animationManager.draw(window, 100, 200);
        window.display();
    }
    return 0;
}