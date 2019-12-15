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

class GameManager
{
  public:
    GameManager()
    {
        level.LoadFromFile("maps/dantuinMap.tmx");
        Player player(jediTexture, level.GetObjects("ground"));
        GameManager::player = player;
        addClone(cloneTexture, level.GetObjects("ground"), {500, 433});
        addClone(cloneTexture, level.GetObjects("ground"), {900, 433});
        addClone(cloneTexture, level.GetObjects("ground"), {1300, 433});
    }

    void update(sf::RenderWindow &window, sf::View &view)
    {
        deltaTime = deltaClock.restart().asMicroseconds() / 1000;
        level.Draw(window);
        player.setView(view, level.GetWindowWidth(), level.tileWidth);
        enemiesUpdate(window, view);
        player.update(deltaTime, window);
    }

  private:
    Texture jediTexture, cloneTexture;
    sf::Clock deltaClock;
    float deltaTime, currentEnemyFrame;
    Player player;
    Level level;
    std::list<Enemy *> enemies;
    std::list<Enemy *>::iterator enemiesIt;
    std::list<Bullet *> bullets;
    std::list<Bullet *>::iterator bulletsIt;

    void addClone(Texture &cloneTexture, vector<Object> GroundObjects, sf::Vector2f position)
    {
        enemies.push_back(new Clone(cloneTexture, GroundObjects, position));
    }

    void addBullet(Vector2f position, bool isEnemyFlip)
    {
        std::cout << "1" << std::endl;
        if (!isEnemyFlip)
        {
            position.x += 10;
        }
        position.y -= 38;
        bullets.push_back(new Bullet(position, isEnemyFlip));
    }

    void bulletsUpdate(RenderWindow &window, sf::View &view, Enemy *enemy)
    {
        int playerSpriteSize = fabs(player.animationManager.getCurrentSpriteSize());
        int enemySpriteSie = fabs(enemy->getAnimationManager()->getCurrentSpriteSize());
        for (bulletsIt = bullets.begin(); bulletsIt != bullets.end(); bulletsIt++)
        {
            Bullet *b = *bulletsIt;
            (*bulletsIt)->update(deltaTime, window, view);
            (*bulletsIt)->draw(window);
            if ((*bulletsIt)->isReflected && (*bulletsIt)->position.x >= enemy->position.x && (*bulletsIt)->position.x <= enemy->position.x + enemySpriteSie && !enemy->isWounded)
            {
                enemy->isWounded = true;
                (*bulletsIt)->isAlive = false;
            }
            if ((*bulletsIt)->position.x >= player.position.x && (*bulletsIt)->position.x <= player.position.x + playerSpriteSize)
            {
                if (player.underAttack && player.isReflecting() && !(*bulletsIt)->isReflected)
                {
                    (*bulletsIt)->changeDirection();
                }
                if (player.underAttack && !player.isReflecting() && !player.isBlocking())
                {
                    player.isAlive = false;
                }
                player.underAttack = true;
            }
            if (!b->isAlive)
            {
                bulletsIt = bullets.erase(bulletsIt);
                delete b;
            }
        }
    }

    void enemiesUpdate(RenderWindow &window, sf::View &view)
    {
        int leftEndOfView = view.getCenter().x - (window.getSize().x / 2) - 100;
        int rightEndOfView = view.getCenter().x + (window.getSize().x / 2) - 100;
        for (enemiesIt = enemies.begin(); enemiesIt != enemies.end(); enemiesIt++)
        {
            Enemy *e = *enemiesIt;
            bulletsUpdate(window, view, (*enemiesIt));
            if (player.onForce() && (*enemiesIt)->position.x > leftEndOfView && (*enemiesIt)->position.x < rightEndOfView)
            {
                (*enemiesIt)->setCurrentAnimation(AnimConfig::TRAP_ANIMATION);
                (*enemiesIt)->isTraped = true;
            }
            if (fabs(player.position.x - (*enemiesIt)->position.x) <= EnemyConfig::VIEW_DISTANCE)
            {
                (*enemiesIt)->setIsAttack(true);
                currentEnemyFrame = (*enemiesIt)->getAnimationManager()->getCurrentFrame();
                if (currentEnemyFrame < 3.1 && currentEnemyFrame > 2.95 && !(*enemiesIt)->isWounded)
                {
                    addBullet((*enemiesIt)->position, (*enemiesIt)->flip);
                    (*enemiesIt)->getAnimationManager()->setCurrentFrame(3.13);
                }
                (*enemiesIt)->flip = player.position.x < (*enemiesIt)->position.x;
            }
            else
            {
                (*enemiesIt)->setIsAttack(false);
            }
            (*enemiesIt)->update(deltaTime);
            (*enemiesIt)->draw(window, deltaTime);
            if (e->isWounded && ((*enemiesIt)->position.x < leftEndOfView || (*enemiesIt)->position.x > rightEndOfView))
            {
                enemiesIt = enemies.erase(enemiesIt);
                delete e;
            }
        }
    }
};

#endif