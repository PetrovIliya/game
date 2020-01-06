#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML\Graphics.hpp>
#include "../../config/AnimConfig.cpp"

using namespace sf;

class Animation
{
  public:
    String name;
    std::vector<IntRect> frames, frames_flip;
    float currentFrame = 0, speed;
    bool flip = false, isPlaying = true;
    Sprite sprite;

    Animation(String Name, Texture &texture, int quantity, float Speed, int x, int y, float step, int width, int height);

    Animation();

    int getSpriteSize();

    int getSpriteHeight();

    int getQuantityOfFrames();

    void setOrigin(int x, int y);

    int getFrameHeight();

    void tick(float time);
};

#endif