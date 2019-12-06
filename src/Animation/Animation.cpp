#include "Animation.hpp"
#include "../config/AnimConfig.cpp"

Animation::Animation (String Name, Texture &texture, int count, float Speed)
{
    name = Name;
    speed = Speed;
    sprite.setTexture(texture);
    currentFrame = 0;
    isPlaying = true;
    flip = true;
    for (int i = 0; i < count; i++)
    {
        frames.push_back(IntRect(i * AnimConfig::STEP, AnimConfig::Y, AnimConfig::WIDTH, AnimConfig::HEIGHT));
        frames_flip.push_back( IntRect(i * AnimConfig::STEP + AnimConfig::WIDTH, AnimConfig::Y, -AnimConfig::WIDTH, AnimConfig::HEIGHT));
    }
}

Animation::Animation()
{}

void Animation::tick(float time)
{
    if (!isPlaying)
    {
        return;
    } 
    currentFrame += speed * time;
    if(name == "walk" && currentFrame >= 6.f)
    {
        currentFrame = AnimConfig::MOVING_FRAME;
    }
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