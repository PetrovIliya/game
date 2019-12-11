#ifndef GAME_GAMEMANAGER_H
#define GAME_GAMEMANAGER_H

#include <ctime>
#include <cmath>
#include "../Player/Player.h"
#include "../EnemyManager/EnemyManager.h"
#include "../Level/Level.h"

class GameManager
{
  public:
    GameManager()
    {
        level.LoadFromFile("maps/dantuinMap.tmx");
        Player player(jediTexture, level.GetObjects("ground"));
        GameManager::player = player;
        enemyManager.addClone(cloneTexture, level.GetObjects("ground"), {500, 433});
    }

    void update(sf::RenderWindow &window, sf::View &view)
    {
        deltaTime = deltaClock.restart().asMicroseconds() / 1000;
        level.Draw(window);
        player.setView(view, level.GetWindowWidth(), level.tileWidth);
        player.update(deltaTime, window);
        bullets.update(deltaTime, window);
        enemyManager.update(window, deltaTime, player.position.x, bullets);
    }

  private:
    Texture jediTexture, cloneTexture;
    sf::Clock deltaClock;
    float deltaTime;
    Player player;
    EnemyManager enemyManager;
    Bullets bullets;
    Level level;

    void enemyHandler()
    {
    }
};

#endif
