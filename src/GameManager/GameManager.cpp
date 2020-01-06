#include "GameManager.h"

GameManager::GameManager(sf::Text playerScore)
{
    level.LoadFromFile("maps/dantuinMap.tmx");
    tileWidth = level.tileWidth;
    Player player(jediTexture, level.GetObjects("ground"), playerScore);
    GameManager::player = player;
    mapOffset = enemyCount = 0;
    paused = wasPaused = false;
}

void GameManager::update(sf::RenderWindow &window, sf::View &view)
{
    int leftEndOfView = view.getCenter().x - (window.getSize().x / 2);
    int rightEndOfView = view.getCenter().x + (window.getSize().x / 2);
    elapsedTime = elapsedClock.getElapsedTime().asSeconds();
    enemyTime = enemyClock.getElapsedTime().asSeconds();
    dletaTimeRestart();
    if (needToReset)
    {
        resetTime();
        needToReset = false;
    }
    enemyGeneration(rightEndOfView, leftEndOfView);
    setMapOffset(view);
    level.Draw(window, mapOffset);
    player.setView(view, level.GetWindowWidth(), level.tileWidth);
    enemiesUpdate(window, view, rightEndOfView, leftEndOfView);
    player.update(deltaTime, window, leftEndOfView);
    wasPaused = false;
}

void GameManager::setPaused(bool paused)
{
    GameManager::paused = paused;
}

void GameManager::resetTime()
{
    deltaTime = elapsedTime = enemyTime = 0;
}

Player GameManager::getPlayer()
{
    return player;
}

void GameManager::enemyGeneration(int rightEndOfView, int leftEndOfView)
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

void GameManager::setMapOffset(sf::View &view)
{
    mapOffset = (view.getCenter().x - WindowConfig::WINDOW_WIDTH / 2) / tileWidth;
}

void GameManager::addClone(Texture &cloneTexture, sf::Vector2f position, int id, bool flip)
{
    enemies.push_back(new Clone(cloneTexture, position, id, flip));
}

void GameManager::addBullet(Vector2f position, bool isEnemyFlip)
{
    if (!isEnemyFlip)
    {
        position.x += 10;
    }
    position.y -= 38;
    bullets.push_back(new Bullet(position, isEnemyFlip));
}

void GameManager::enemiesUpdate(RenderWindow &window, sf::View &view, int rightEndOfView, int leftEndOfView)
{
    for (enemiesIt = enemies.begin(); enemiesIt != enemies.end(); enemiesIt++)
    {
        int enemySpriteHeight = (*enemiesIt)->getCurrentSpriteHeight();
        int playerSpriteSize = fabs(player.getCurrentSpriteSize());
        int enemySpriteSie = fabs((*enemiesIt)->getCurrentSpriteSize());
        enemyEnemyColissionHandler(*enemiesIt, enemySpriteSie);
        bulletsUpdate(window, view, (*enemiesIt), playerSpriteSize, enemySpriteSie);
        enemyStasisHandler(*enemiesIt, leftEndOfView, rightEndOfView);
        enemyLifeHandler(*enemiesIt);
        enemyAttackHandler(*enemiesIt);
        enemyPlayerCollisionHandler(*enemiesIt, playerSpriteSize, enemySpriteSie, enemySpriteHeight);
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

void GameManager::bulletsUpdate(RenderWindow &window, sf::View &view, Enemy *enemy, int playerSpriteSize, int enemySpriteSie)
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

void GameManager::playerHitHandler(Bullet *bullet, int playerSpriteSize)
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

void GameManager::enemyHitHandler(Enemy *enemy, Bullet *bullet, int enemySpriteSie)
{
    if (isEnemyHited(enemy, bullet, enemySpriteSie))
    {
        player.score++;
        enemy->isWounded = true;
        bullet->isAlive = false;
    }
}

void GameManager::enemyPlayerCollisionHandler(Enemy *enemy, int playerSpriteSize, int enemySpriteSie, int emenySpriteHeight)
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

void GameManager::enemyEnemyColissionHandler(Enemy *enemy, int enemySpriteSie)
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

void GameManager::enemyStasisHandler(Enemy *enemy, int leftEndOfView, int rightEndOfView)
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

void GameManager::enemyLifeHandler(Enemy *enemy)
{
    if (player.flip)
    {
        if ((inRightAttackDistance(enemy) && player.isAttack() && !enemy->isWounded))
        {
            player.score += 5;
            enemy->isWounded = true;
        }
    }
    else
    {
        if (inLeftAttackDistance(enemy) && player.isAttack() && !enemy->isWounded)
        {
            player.score += 5;
            enemy->isWounded = true;
        }
    }
}

void GameManager::enemyAttackHandler(Enemy *enemy)
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

void GameManager::dletaTimeRestart()
{
    deltaTime = deltaClock.restart().asMicroseconds() / 1000;
}

bool GameManager::isEnemyHited(Enemy *enemy, Bullet *bullet, int enemySpriteSie)
{
    return bullet->isReflected && bullet->position.x >= enemy->position.x && bullet->position.x <= enemy->position.x + enemySpriteSie && !enemy->isWounded;
}

bool GameManager::inLeftAttackDistance(Enemy *enemy)
{
    return player.position.x + PlayerConfig::ATTACK_DISTANCE >= enemy->position.x && player.position.x < enemy->position.x;
}

bool GameManager::inRightAttackDistance(Enemy *enemy)
{
    return player.position.x - PlayerConfig::ATTACK_DISTANCE - 5 <= enemy->position.x && player.position.x > enemy->position.x;
}

bool GameManager::isTopColission(Enemy *enemy, int emenySpriteHeight)
{
    return player.position.y > enemy->position.y - emenySpriteHeight;
}

bool GameManager::isLeftColission(Enemy *enemy, int playerSpriteSize)
{
    return enemy->position.x >= player.position.x && enemy->position.x <= player.position.x + playerSpriteSize | player.position.x <= 0;
}

bool GameManager::isRightColission(Enemy *enemy, int enemySpriteSie)
{
    return player.position.x >= enemy->position.x && player.position.x <= enemy->position.x + enemySpriteSie | player.position.x >= 16000;
}