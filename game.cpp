#include "src/Game/Game.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({WindowConfig::WINDOW_WIDTH, WindowConfig::WINDOW_HEIGHT}), "Star wars");
    sf::View view;
    view.setCenter(WindowConfig::WINDOW_WIDTH / 2, WindowConfig::WINDOW_HEIGHT / 2);
    view.setSize(WindowConfig::WINDOW_WIDTH, WindowConfig::WINDOW_HEIGHT);
    sf::Font font;
    font.loadFromFile("CyrilicOld.TTF");
    sf::Text text("", font, 30);
    text.setString("your score: 0  record: 0");
    text.setStyle(sf::Text::Bold);
    text.setFillColor(sf::Color::Red);
    Game game(text);
    game.start(window, view);
    return 0;
}