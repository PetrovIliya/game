#include "Game.h"

Game::Game(sf::Text playerScore)
{
    Game::playerScore = playerScore;
    enter = false;
    currentScreen = "menu";
    screens["game"] = new GameManager(Game::playerScore);
    screens["menu"] = &menu;
}

void Game::start(sf::RenderWindow &window, sf::View &view)
{
    window.setMouseCursorVisible(false);
    while (window.isOpen())
    {
        pollEvents(window);
        choseHandler(window, view);
        playerDeathHandler();
        window.clear(Color::White);
        window.setView(view);
        screens[currentScreen]->update(window, view);
        window.display();
    }
}

void Game::playerDeathHandler()
{
    if (!screens["game"]->getPlayer().isAlive)
    {
        currentScreen = "menu";
    }
}

void Game::pollEvents(sf::RenderWindow &window)
{
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                screenHandler(window);
                menuHandler();
                break;
            default:
                break;
        }
    }
}

void Game::choseHandler(sf::RenderWindow &window, sf::View &view)
{
    if (enter)
    {
        enter = false;
        if(menu.activeSpriteIndex == 0 && screens["game"]->getPlayer().isAlive)
        {
            currentScreen = "game";
        }
        if(menu.activeSpriteIndex == 1)
        {
            screens["game"] = new GameManager(Game::playerScore);
            Game::screens["game"]->getPlayer().recordHandler();
            currentScreen = "game";
            view.setCenter(WindowConfig::WINDOW_WIDTH / 2, WindowConfig::WINDOW_HEIGHT / 2);
        }
        if(menu.activeSpriteIndex == 2)
        {
            Game::screens["game"]->getPlayer().recordHandler();
            window.close();
        }
    }
}

void Game::menuHandler()
{
    if (event.key.code == sf::Keyboard::Up && currentScreen == "menu")
    {
        menu.movingUp = true;
    }
    if (event.key.code == sf::Keyboard::Down && currentScreen == "menu")
    {
        menu.movingDown = true;
    }
    if (event.key.code == sf::Keyboard::Return && currentScreen == "menu")
    {
        enter = true;
    }
}

void Game::screenHandler(sf::RenderWindow &window)
{
    if (event.key.code == sf::Keyboard::Escape)
    {
        if (currentScreen == "game")
        {
            currentScreen = "menu";
            window.setMouseCursorVisible(true);
            screens["game"]->needToReset = true;
        }
        else
        {
            window.setMouseCursorVisible(false);
            currentScreen = "game";
            screens["game"]->needToReset = true;
        }
    }
}