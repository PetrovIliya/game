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

    Animation(String Name, Texture &texture, int quantity, float Speed, int x, int y, float step, int width, int height)
    {
        name = Name;
        speed = Speed;
        sprite.setTexture(texture);
        for (int i = 0; i < quantity; i++)
        {
            frames.push_back(IntRect(i * step + x, y, width, height));
            frames_flip.push_back(IntRect(i * step + width + x, y, -width, height));
        }
    }

    Animation()
    {
    }

    void tick(float time)
    {
        if (!isPlaying)
        {
            return;
        }
        currentFrame += speed * time;
        FrameLoopHandler();
        if (currentFrame > frames.size())
        {
            currentFrame -= frames.size();
        }
        int i = currentFrame;
        sprite.setTextureRect(frames[i]);
        if (flip)
        {
            sprite.setTextureRect(frames_flip[i]);
        }
    }

  private:
    void FrameLoopHandler()
    {
        if (name == AnimConfig::WALK_ANIMATION && currentFrame >= 9)
        {
            currentFrame = 3;
        }

        if (name == AnimConfig::DEATH_NAME && currentFrame >= 10)
        {
            currentFrame = 9;
        }
    }
};

#endif