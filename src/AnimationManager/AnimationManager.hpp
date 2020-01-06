#ifndef ANIMATION_MANAGER_H
#define ANIMATION_MANAGER_H

#include <SFML\Graphics.hpp>
#include "../Animation/Animation.hpp"
#include "../../tinyxml/tinyxml.h"

class AnimationManager
{
  public:
    String currentAnim;
    std::map<String, Animation> animList;

    void loadFromXml(std::string fileName, Texture &texture);

    void setPosition(sf::Vector2f position);

    float getCurrentFrame();

    void setCurrentFrame(float currentFrame);

    Animation getCurrentAnimation();

    int getCurrentSpriteSize();

    int getCurrentSpriteHeight();

    bool isLastFrame();

    int getQuantityOfFrames();

    void draw(RenderWindow &window);

    void resetAnimation();

    void set(String name);

    void flip(bool isFlip);

    void tick(float time);

    void pause();

    void play();

    void setFrameOrigin();

    String getCurrentAnimationName();

  private:
    void setAnimationsFromFile(TiXmlElement *animElement, Texture &texture);

    void setFramesFromFile(TiXmlElement *animElement, Animation &animation);
};

#endif
