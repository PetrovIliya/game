#ifndef ANIM_CONFIG
#define ANIM_CONFIG

class AnimConfig
{
  public:
    static const int MID_MOVING_FRAME = 3;
    static const int END_MOVING_FRAME = 6;
    static const int JUMP_HEIGHT = 70;
    inline static const sf::String WALK_ANIMATION = "walk";
    inline static const sf::String ATTACK_ANIMATION = "attack";
    inline static const sf::String STAY_ANIMATION = "stay";
    inline static const sf::String BLOCK_ANIMATION = "block";
    inline static const sf::String JUMP_ANIMATION = "jump";
    inline static const sf::String DEATH_ANIMATION = "death";
    inline static const sf::String FORCE_ANIMATION = "force";
    inline static const sf::String REFLECT_ANIMATION = "reflect";
    inline static const sf::String JEDI_TEXTURE_PATH = "textures/jedi.png";
};

#endif