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
        telekinesisSpeed = 0.2f;
        level.LoadFromFile("maps/dantuinMap.tmx");
        Player player(jediTexture, level.GetObjects("ground"));
        GameManager::player = player;
        addClone(cloneTexture, level.GetObjects("ground"), {500, 433});
        addClone(cloneTexture, level.GetObjects("ground"), {900, 433});
        addClone(cloneTexture, level.GetObjects("ground"), {1300, 433});
    }

    void update(sf::RenderWindow &window, sf::View &view)
    {
        elapsedTime = elapsedClock.getElapsedTime().asSeconds();
        deltaTime = deltaClock.restart().asMicroseconds() / 1000;
        level.Draw(window);
        player.setView(view, level.GetWindowWidth(), level.tileWidth);
        enemiesUpdate(window, view);
        player.update(deltaTime, window);
    }

  private:
    Texture jediTexture, cloneTexture;
    sf::Clock deltaClock, elapsedClock;
    float deltaTime, elapsedTime, currentEnemyFrame, telekinesisSpeed;
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
        if (!isEnemyFlip)
        {
            position.x += 10;
        }
        position.y -= 38;
        bullets.push_back(new Bullet(position, isEnemyFlip));
    }

    void enemiesUpdate(RenderWindow &window, sf::View &view)
    {
        int leftEndOfView = view.getCenter().x - (window.getSize().x / 2);
        int rightEndOfView = view.getCenter().x + (window.getSize().x / 2);
        for (enemiesIt = enemies.begin(); enemiesIt != enemies.end(); enemiesIt++)
        {
            int emenySpriteHeight = (*enemiesIt)->getCurrentSpriteHeight();
            int playerSpriteSize = fabs(player.getCurrentSpriteSize());
            int enemySpriteSie = fabs((*enemiesIt)->getCurrentSpriteSize());
            bulletsUpdate(window, view, (*enemiesIt), playerSpriteSize, enemySpriteSie);
            eneymyStasisHandler(*enemiesIt, leftEndOfView, rightEndOfView);
            enemyLifeHandler(*enemiesIt);
            enemyAttackHandler(*enemiesIt);
            enemyCollisionHandler(*enemiesIt, playerSpriteSize, enemySpriteSie, emenySpriteHeight);
            (*enemiesIt)->update(elapsedTime, elapsedClock);
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

    void enemyCollisionHandler(Enemy *enemy, int playerSpriteSize, int enemySpriteSie, int emenySpriteHeight)
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

    void eneymyStasisHandler(Enemy *enemy, int leftEndOfView, int rightEndOfView)
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
        return player.position.x - PlayerConfig::ATTACK_DISTANCE <= enemy->position.x && player.position.x > enemy->position.x;
    }

    bool isTopColission(Enemy *enemy, int emenySpriteHeight)
    {
        return player.position.y > enemy->position.y - emenySpriteHeight;
    }

    bool isLeftColission(Enemy *enemy, int playerSpriteSize)
    {
        return enemy->position.x >= player.position.x && enemy->position.x <= player.position.x + playerSpriteSize;
    }

    bool isRightColission(Enemy *enemy, int enemySpriteSie)
    {
        return player.position.x >= enemy->position.x && player.position.x <= enemy->position.x + enemySpriteSie;
    }
};

#endif