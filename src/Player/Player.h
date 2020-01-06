#ifndef PLAYER_H
#define PLAYER_H

#include "../AnimationManager/AnimationManager.hpp"
#include "../Level/Level.h"
#include "../../config/WindowConfig.hpp"
#include "../../config/PlayerConfig.cpp"
#include <fstream>
#include <sstream>

using namespace std;

class Player
{
  public:
    AnimationManager animationManager;
    bool attack, flip, onGround, onJump, canMove, keyPressed, underAttack, isAlive, onCoolDown, canUseForce, ableToMoveRight, ableToMoveLeft;
    sf::Vector2f position, lastPosition;
    vector<Object> groundObjects;
    Object mainGroundObject;
    sf::Clock elapsedClock;
    float elapsedTime;
    sf::Text playerScore;
    int score = 0;
    std::string record = "0";


    Player(Texture &jediTexture, vector<Object> GroundObjects, sf::Text playerScore);

    Player()
    {
    }

    void update(float time, sf::RenderWindow &window, int leftEndOfView);

    void scoreHandler(int leftEndOfView);

    void setView(sf::View &view, int realWindowWidth, int tileWidth);

    void restartTime();

    void createFile();

    void coolDownHandler();

    bool ableToMove();

    void setPosition(Vector2f position);

    bool isReflecting();

    bool isBlocking();

    bool onForce();

    bool isAttack();

    int getCurrentSpriteSize();

    void recordHandler();


private:
    void initObjects(vector<Object> GroundObjects);

    void lifeHandler();

    void moveHandler(float time);

    void reflectHandler();

    void blockHandler();

    void jumpHandler(float time);

    void forceHandler();

    void attackHandler();

    void setWalkAnimation();

    void setStayAnimation();

    void setBlockAnimation();

    void setReflectAnimation();

    void setAttackAnimation();

    void setFroceAnimation();

    void setJumpAnimation();

    void setFallAnimation();

    sf::String getCurrentAnimName();

    void resetAnimation();

    void setWalkAnimationAndDirection(bool flip);
};

#endif