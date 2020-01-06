#include "AnimationManager.hpp"

void AnimationManager::loadFromXml(std::string fileName, Texture &texture)
{
    TiXmlDocument animFile(fileName.c_str());
    animFile.LoadFile();
    TiXmlElement *head;
    head = animFile.FirstChildElement("sprites");
    TiXmlElement *animElement;
    animElement = head->FirstChildElement("animation");
    setAnimationsFromFile(animElement, texture);
}

void AnimationManager::setPosition(sf::Vector2f position)
{
    animList[currentAnim].sprite.setPosition(position);
}

float AnimationManager::getCurrentFrame()
{
    return animList[currentAnim].currentFrame;
}

void AnimationManager::setCurrentFrame(float currentFrame)
{
    animList[currentAnim].currentFrame = currentFrame;
}

Animation AnimationManager::getCurrentAnimation()
{
    return animList[currentAnim];
}

int AnimationManager::getCurrentSpriteSize()
{
    return animList[currentAnim].getSpriteSize();
}

int AnimationManager::getCurrentSpriteHeight()
{
    return animList[currentAnim].getSpriteHeight();
}

bool AnimationManager::isLastFrame()
{
    return getCurrentFrame() >= (float)getQuantityOfFrames() - 0.1;
}

int AnimationManager::getQuantityOfFrames()
{
    return animList[currentAnim].getQuantityOfFrames();
}

void AnimationManager::draw(RenderWindow &window)
{
    setFrameOrigin();
    window.draw(animList[currentAnim].sprite);
}

void AnimationManager::resetAnimation()
{
    animList[currentAnim].currentFrame = 0;
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

void AnimationManager::setFrameOrigin()
{
    int frameHeight = animList[currentAnim].getFrameHeight();
    animList[currentAnim].setOrigin(0, frameHeight);
}

String AnimationManager::getCurrentAnimationName()
{
    return currentAnim;
}

void AnimationManager::setAnimationsFromFile(TiXmlElement *animElement, Texture &texture)
{
    while (animElement)
    {
        Animation animation;
        currentAnim = animElement->Attribute("title");
        int delay = atoi(animElement->Attribute("delay"));
        animation.speed = 1.0 / delay;
        animation.sprite.setTexture(texture);
        setFramesFromFile(animElement, animation);
        animList[currentAnim] = animation;
        animElement = animElement->NextSiblingElement("animation");
    }
}

void AnimationManager::setFramesFromFile(TiXmlElement *animElement, Animation &animation)
{
    TiXmlElement *cut;
    cut = animElement->FirstChildElement("cut");
    while (cut)
    {
        int x = atoi(cut->Attribute("x"));
        int y = atoi(cut->Attribute("y"));
        int width = atoi(cut->Attribute("w"));
        int height = atoi(cut->Attribute("h"));

        animation.frames.push_back(IntRect(x, y, width, height));
        animation.frames_flip.push_back(IntRect(x + width, y, -width, height));
        cut = cut->NextSiblingElement("cut");
    }
}