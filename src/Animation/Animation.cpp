#include "Animation.hpp"

Animation::Animation(String Name, Texture &texture, int quantity, float Speed, int x, int y, float step, int width, int height)
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

Animation::Animation()
{}

int Animation::getSpriteSize()
{
    return sprite.getTextureRect().width;
}

int Animation::getSpriteHeight()
{
    return sprite.getTextureRect().height;
}

int Animation::getQuantityOfFrames()
{
    return frames.size();
}

void Animation::setOrigin(int x, int y)
{
    sprite.setOrigin(x, y);
}

int Animation::getFrameHeight()
{
    return frames[currentFrame].height;
}

void Animation::tick(float time)
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