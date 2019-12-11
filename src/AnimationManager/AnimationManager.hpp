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

    void loadFromXml(std::string fileName, Texture &texture)
    {
        TiXmlDocument animFile(fileName.c_str());
        animFile.LoadFile();
        TiXmlElement *head;
        head = animFile.FirstChildElement("sprites");
        TiXmlElement *animElement;
        animElement = head->FirstChildElement("animation");
        setAnimationsFromFile(animElement, texture);
    }

    void setPosition(sf::Vector2f position)
    {
        animList[currentAnim].sprite.setPosition(position);
    }

    float getCurrentFrame()
    {
        return animList[currentAnim].currentFrame;
    }

    void draw(RenderWindow &window)
    {
        setFrameOrigin();
        window.draw(animList[currentAnim].sprite);
    }

    void resetAnimation()
    {
        animList[currentAnim].currentFrame = 0;
    }

    void set(String name)
    {
        currentAnim = name;
    }

    void flip(bool isFlip)
    {
        animList[currentAnim].flip = isFlip;
    }

    void tick(float time)
    {
        animList[currentAnim].tick(time);
    }

    void pause()
    {
        animList[currentAnim].isPlaying = false;
    }

    void play()
    {
        animList[currentAnim].isPlaying = true;
    }

    void setFrameOrigin()
    {
        int frameHeight = animList[currentAnim].frames[animList[currentAnim].currentFrame].height;
        animList[currentAnim].sprite.setOrigin(0, frameHeight);
    }

    String getCurrentAnim()
    {
        return currentAnim;
    }

  private:
    void setAnimationsFromFile(TiXmlElement *animElement, Texture &texture)
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

    void setFramesFromFile(TiXmlElement *animElement, Animation &animation)
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
};

#endif