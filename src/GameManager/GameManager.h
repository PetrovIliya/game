#ifndef GAME_GAMEMANAGER_H
#define GAME_GAMEMANAGER_H

#include <ctime>
#include <cmath>
#include <list>
#include "../Player/Player.h"
#include "../Level/Level.h"
#include "../Bullet/Bullet.h"
#include "../Enemy/Enemy.h"
#include "../Clone/Clone.h"
#include "../../config/EnemyConfig.cpp"
#include "../Screen/Screen.h"

class GameManager : public Screen
{
public:
  GameManager(sf::Text playerScore);

  void update(sf::RenderWindow &window, sf::View &view, bool isPlayerAlive);

  void setPaused(bool paused);

  void resetTime();

  Player getPlayer();

private:
  bool paused, wasPaused;
  int mapOffset, tileWidth, enemyCount;
  Texture jediTexture, cloneTexture;
  sf::Clock deltaClock, elapsedClock, enemyClock;
  float deltaTime, elapsedTime, enemyTime, currentEnemyFrame;
  Player player;
  Level level;
  std::list<Enemy *> enemies;
  std::list<Enemy *>::iterator enemiesIt;
  std::list<Enemy *>::iterator enemiesIt2;
  std::list<Bullet *> bullets;
  std::list<Bullet *>::iterator bulletsIt;

  void
  enemyGeneration(int rightEndOfView, int leftEndOfView);

  void setMapOffset(sf::View &view);

  void addClone(Texture &cloneTexture, sf::Vector2f position, int id, bool flip);

  void addBullet(Vector2f position, bool isEnemyFlip);

  void enemiesUpdate(RenderWindow &window, sf::View &view, int rightEndOfView, int leftEndOfView);

  void bulletsUpdate(RenderWindow &window, sf::View &view, Enemy *enemy, int playerSpriteSize, int enemySpriteSie);

  void playerHitHandler(Bullet *bullet, int playerSpriteSize);

  void enemyHitHandler(Enemy *enemy, Bullet *bullet, int enemySpriteSie);

  void enemyPlayerCollisionHandler(Enemy *enemy, int playerSpriteSize, int enemySpriteSie, int emenySpriteHeight);

  void enemyEnemyColissionHandler(Enemy *enemy, int enemySpriteSie);

  void enemyStasisHandler(Enemy *enemy, int leftEndOfView, int rightEndOfView);

  void enemyLifeHandler(Enemy *enemy);

  void enemyAttackHandler(Enemy *enemy);

  void dletaTimeRestart();

  bool isEnemyHited(Enemy *enemy, Bullet *bullet, int enemySpriteSie);

  bool inLeftAttackDistance(Enemy *enemy);

  bool inRightAttackDistance(Enemy *enemy);

  bool isTopColission(Enemy *enemy, int emenySpriteHeight);

  bool isLeftColission(Enemy *enemy, int playerSpriteSize);

  bool isRightColission(Enemy *enemy, int enemySpriteSie);
};

#endif
