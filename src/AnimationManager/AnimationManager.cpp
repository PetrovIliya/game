#include "AnimationManager.hpp"

using namespace sf;

AnimationManager::AnimationManager()
{}
    
void AnimationManager::create(String name, Texture &texture, int count, float speed)
{
    animList[name] = Animation(name, texture, count, speed);
    currentAnim = name;
}
void AnimationManager::draw(RenderWindow &window, int x=0, int y=0)
{
    animList[currentAnim].sprite.setPosition(x,y);
    window.draw( animList[currentAnim].sprite );
}

void AnimationManager::set(String name)
{
     currentAnim = name;
}

void AnimationManager::flip(bool isFlip) 
{
    animList[currentAnim].flip = isFlip;
}

void AnimationManager::tick(float time)
{
    animList[currentAnim].tick(time);
}

void AnimationManager::pause() 
{
    animList[currentAnim].isPlaying = false;
}

void AnimationManager::play()
{
    animList[currentAnim].isPlaying = true;
}