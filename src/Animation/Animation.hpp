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

    int getSpriteSize()
    {
        return sprite.getTextureRect().width;
    }

    int getSpriteHeight()
    {
        return sprite.getTextureRect().height;
    }

    int getQuantityOfFrames()
    {
        return frames.size();
    }

    void setOrigin(int x, int y)
    {
        sprite.setOrigin(x, y);
    }

    int getFrameHeight()
    {
        return frames[currentFrame].height;
    }

    void tick(float time)
    {
        if (!isPlaying)
        {
            return;
        }
        currentFrame += speed * time;
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
};

#endif