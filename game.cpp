#include "src/Game/Game.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({WindowConfig::WINDOW_WIDTH, WindowConfig::WINDOW_HEIGHT}), "Star wars");
    sf::View view;
    view.setCenter(WindowConfig::WINDOW_WIDTH / 2, WindowConfig::WINDOW_HEIGHT / 2);
    view.setSize(WindowConfig::WINDOW_WIDTH, WindowConfig::WINDOW_HEIGHT);
    Game game;
    game.start(window, view);
    return 0;
}