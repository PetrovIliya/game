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

    void create(String name, Texture &texture, int count, float speed, int x, int y, float step, int width, int height)
    {
        animList[name] = Animation(name, texture, count, speed, x, y, step, width, height);
        currentAnim = name;
    }

    void loadFromXml(std::string fileName, Texture &texture)
    {
        TiXmlDocument animFile(fileName.c_str());
        animFile.LoadFile();

        TiXmlElement *head;
        head = animFile.FirstChildElement("sprites");
        TiXmlElement *animElement;
        animElement = head->FirstChildElement("animation");
        while (animElement)
        {
            Animation animation;
            currentAnim = animElement->Attribute("title");
            int delay = atoi(animElement->Attribute("delay"));
            animation.speed = 1.0 / delay;
            animation.sprite.setTexture(texture);
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
            animList[currentAnim] = animation;
            animElement = animElement->NextSiblingElement("animation");
        }
    }

    void draw(RenderWindow &window, int x = 0, int y = 0)
    {
        animList[currentAnim].sprite.setPosition(x, y);
        window.draw(animList[currentAnim].sprite);
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
};

#endif