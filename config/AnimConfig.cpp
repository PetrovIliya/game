#ifndef ANIM_CONFIG
#define ANIM_CONFIG

class AnimConfig
{
public:
    static const int HEIGHT = 70;
    static const int WIDTH = 37;
    static const int STEP = 35;
    static const int Y = 0;
    static const int MID_MOVING_FRAME = 3;
    static const int END_MOVING_FRAME = 6;
    inline static const sf::String WALK_ANIMATION = "walk";
    inline static const sf::String ATTACK_ANIMATION = "attack";
    inline static const sf::String STAY_ANIMATION = "stay";
    inline static const sf::String JUMP_NAME = "jump";
    inline static const sf::String DEATH_NAME = "death";
    inline static const sf::String LIGHTNING_NAME = "force";
    inline static const sf::String JEDI_TEXTURE_PATH = "textures/jedi.png";
    inline static const sf::String VADER_TEXTURE_PATH = "textures/jedi.png";
};

#endif