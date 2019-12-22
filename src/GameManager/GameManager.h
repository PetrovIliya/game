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
        tileWidth = level.tileWidth;
        Player player(jediTexture, level.GetObjects("ground"));
        GameManager::player = player;
        mapOffset = enemyCount = 0;
        paused = wasPaused = false;
    }

    void update(sf::RenderWindow &window, sf::View &view)
    {

        if (!paused)
        {
            pollEvents(window);

            window.clear(Color::White);
            int leftEndOfView = view.getCenter().x - (window.getSize().x / 2);
            int rightEndOfView = view.getCenter().x + (window.getSize().x / 2);
            elapsedTime = elapsedClock.getElapsedTime().asSeconds();
            enemyTime = enemyClock.getElapsedTime().asSeconds();
            deltaTime = deltaClock.restart().asMicroseconds() / 1000;
            if (wasPaused)
            {
                deltaTime = elapsedTime = enemyTime = 0;
            }
            std::cout << deltaTime << std::endl;
            enemyGeneration(rightEndOfView, leftEndOfView);
            setMapOffset(view);
            level.Draw(window, mapOffset);
            player.setView(view, level.GetWindowWidth(), level.tileWidth);
            enemiesUpdate(window, view, rightEndOfView, leftEndOfView);
            player.update(deltaTime, window);
            wasPaused = false;
        }
    }

    void setPaused(bool paused)
    {
        GameManager::paused = paused;
    }

  private:
    sf::Event event;
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

    void pollEvents(sf::RenderWindow &window)
    {
        while (window.pollEvent(event) || paused)
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                pauseHandler();
                break;
            default:
                break;
            }
        }
    }

    pauseHandler()
    {
        if (event.key.code == sf::Keyboard::Escape)
        {
            paused = false;
        }
        if (event.key.code == sf::Keyboard::Escape && !paused && !wasPaused)
        {
            paused = true;
            wasPaused = true;
            deltaClock.restart();
        }
    }

    void enemyGeneration(int rightEndOfView, int leftEndOfView)
    {
        if (enemyTime >= 5)
        {
            for (int i = 0; i < 3; i++)
            {
                Vector2f newPosition = {float(rightEndOfView + i * 200), 448};
                addClone(cloneTexture, newPosition, enemyCount, true);
                enemyCount++;
            }
            enemyClock.restart();
        }
    }

    void setMapOffset(sf::View &view)
    {
        mapOffset = (view.getCenter().x - WindowConfig::WINDOW_WIDTH / 2) / tileWidth;
    }

    void addClone(Texture &cloneTexture, sf::Vector2f position, int id, bool flip)
    {
        enemies.push_back(new Clone(cloneTexture, position, id, flip));
    }

    void addBullet(Vector2f position, bool isEnemyFlip)
    {
        if (!isEnemyFlip)
        {
            position.x += 10;
        }
        position.y -= 38;
        bullets.push_back(new Bullet(position, isEnemyFlip));
    }

    void enemiesUpdate(RenderWindow &window, sf::View &view, int rightEndOfView, int leftEndOfView)
    {
        for (enemiesIt = enemies.begin(); enemiesIt != enemies.end(); enemiesIt++)
        {
            int emenySpriteHeight = (*enemiesIt)->getCurrentSpriteHeight();
            int playerSpriteSize = fabs(player.getCurrentSpriteSize());
            int enemySpriteSie = fabs((*enemiesIt)->getCurrentSpriteSize());
            enemyEnemyColissionHandler(*enemiesIt, enemySpriteSie);
            bulletsUpdate(window, view, (*enemiesIt), playerSpriteSize, enemySpriteSie);
            enemyStasisHandler(*enemiesIt, leftEndOfView, rightEndOfView);
            enemyLifeHandler(*enemiesIt);
            enemyAttackHandler(*enemiesIt);
            enemyPlayerCollisionHandler(*enemiesIt, playerSpriteSize, enemySpriteSie, emenySpriteHeight);
            (*enemiesIt)->update(elapsedTime, elapsedClock, deltaTime, player.position.x);
            (*enemiesIt)->draw(window, deltaTime);
            Enemy *e = *enemiesIt;
            if (e->isWounded && ((*enemiesIt)->position.x < leftEndOfView || (*enemiesIt)->position.x > rightEndOfView))
            {
                enemiesIt = enemies.erase(enemiesIt);
                delete e;
            }
        }
    }

    void bulletsUpdate(RenderWindow &window, sf::View &view, Enemy *enemy, int playerSpriteSize, int enemySpriteSie)
    {
        for (bulletsIt = bullets.begin(); bulletsIt != bullets.end(); bulletsIt++)
        {
            (*bulletsIt)->update(deltaTime, window, view);
            (*bulletsIt)->draw(window);
            playerHitHandler(*bulletsIt, playerSpriteSize);
            enemyHitHandler(enemy, *bulletsIt, enemySpriteSie);
            Bullet *b = *bulletsIt;
            if (!b->isAlive)
            {
                bulletsIt = bullets.erase(bulletsIt);
                delete b;
            }
        }
    }

    void playerHitHandler(Bullet *bullet, int playerSpriteSize)
    {
        if (bullet->position.x >= player.position.x && bullet->position.x <= player.position.x + playerSpriteSize && player.position.y > bullet->position.y)
        {
            if (player.underAttack && player.isReflecting() && !bullet->isReflected)
            {
                bullet->changeDirection();
            }
            if (player.underAttack && !player.isReflecting() && !player.isBlocking())
            {
                player.isAlive = false;
            }
            player.underAttack = true;
        }
    }

    void enemyHitHandler(Enemy *enemy, Bullet *bullet, int enemySpriteSie)
    {
        if (isEnemyHited(enemy, bullet, enemySpriteSie))
        {
            enemy->isWounded = true;
            bullet->isAlive = false;
        }
    }

    void enemyPlayerCollisionHandler(Enemy *enemy, int playerSpriteSize, int enemySpriteSie, int emenySpriteHeight)
    {
        if (isLeftColission(enemy, playerSpriteSize) | isRightColission(enemy, enemySpriteSie) && isTopColission(enemy, emenySpriteHeight) && !enemy->isWounded)
        {
            if (player.flip && player.ableToMoveRight)
            {
                player.ableToMoveLeft = false;
                enemy->onColission = true;
            }
            if (!player.flip && player.ableToMoveLeft)
            {
                player.ableToMoveRight = false;
                enemy->onColission = true;
            }
        }
        else if (enemy->onColission)
        {
            player.ableToMoveLeft = true;
            player.ableToMoveRight = true;
            enemy->onColission = false;
        }
    }

    void enemyEnemyColissionHandler(Enemy *enemy, int enemySpriteSie)
    {
        for (enemiesIt2 = enemies.begin(); enemiesIt2 != enemies.end(); enemiesIt2++)
        {
            if (enemy->id == (*enemiesIt2)->id)
            {
                continue;
            }
            if (enemy->position.x >= (*enemiesIt2)->position.x && enemy->position.x <= (*enemiesIt2)->position.x + enemySpriteSie)
            {
                enemy->ableToMoveLeft = false;
                (*enemiesIt2)->ableToMoveRight = false;
            }
            else
            {
                enemy->ableToMoveLeft = true;
                (*enemiesIt2)->ableToMoveRight = true;
            }
        }
    }

    void enemyStasisHandler(Enemy *enemy, int leftEndOfView, int rightEndOfView)
    {
        if (enemy->position.x > leftEndOfView && enemy->position.x < rightEndOfView && !enemy->isWounded)
        {
            if (player.onForce())
            {
                enemy->setCurrentAnimation(AnimConfig::TRAP_ANIMATION);
                enemy->isTraped = true;
            }
        }
    }

    void enemyLifeHandler(Enemy *enemy)
    {
        if (player.flip)
        {
            if ((inRightAttackDistance(enemy) && player.isAttack() && !enemy->isWounded))
            {
                enemy->isWounded = true;
            }
        }
        else
        {
            if (inLeftAttackDistance(enemy) && player.isAttack() && !enemy->isWounded)
            {
                enemy->isWounded = true;
            }
        }
    }

    void enemyAttackHandler(Enemy *enemy)
    {
        if (fabs(player.position.x - enemy->position.x) <= EnemyConfig::VIEW_DISTANCE)
        {
            enemy->setIsAttack(true);
            currentEnemyFrame = enemy->getAnimationManager()->getCurrentFrame();
            if (currentEnemyFrame < 3.1 && currentEnemyFrame > 2.94 && !enemy->isWounded)
            {
                addBullet(enemy->position, enemy->flip);
                enemy->getAnimationManager()->setCurrentFrame(3.13);
            }
            enemy->flip = player.position.x < enemy->position.x;
        }
        else
        {
            enemy->setIsAttack(false);
        }
    }

    bool isEnemyHited(Enemy *enemy, Bullet *bullet, int enemySpriteSie)
    {
        return bullet->isReflected && bullet->position.x >= enemy->position.x && bullet->position.x <= enemy->position.x + enemySpriteSie && !enemy->isWounded;
    }

    bool inLeftAttackDistance(Enemy *enemy)
    {
        return player.position.x + PlayerConfig::ATTACK_DISTANCE >= enemy->position.x && player.position.x < enemy->position.x;
    }

    bool inRightAttackDistance(Enemy *enemy)
    {
        return player.position.x - PlayerConfig::ATTACK_DISTANCE - 5 <= enemy->position.x && player.position.x > enemy->position.x;
    }

    bool isTopColission(Enemy *enemy, int emenySpriteHeight)
    {
        return player.position.y > enemy->position.y - emenySpriteHeight;
    }

    bool isLeftColission(Enemy *enemy, int playerSpriteSize)
    {
        return enemy->position.x >= player.position.x && enemy->position.x <= player.position.x + playerSpriteSize | player.position.x <= 0;
    }

    bool isRightColission(Enemy *enemy, int enemySpriteSie)
    {
        return player.position.x >= enemy->position.x && player.position.x <= enemy->position.x + enemySpriteSie | player.position.x >= 16000;
    }
};

#endif