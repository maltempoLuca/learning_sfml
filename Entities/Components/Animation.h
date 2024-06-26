#ifndef SFML_ANIMATION_H
#define SFML_ANIMATION_H

#include<map>
#include<iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

class Animation {
public:
    //Variables
    sf::Sprite &sprite;
    sf::Texture &textureSheet;
    /**
     * Durata di una singola texture dell'animazione
     */
    float animationTimer;
    float timer;
    int width;
    int height;
    sf::IntRect startRect;
    sf::IntRect currentRect;
    sf::IntRect endRect;
    bool canBeInterrupted = true;

    Animation(sf::Sprite &sprite, sf::Texture &texture_sheet, float animation_timer, int start_frame_x,
              int start_frame_y, int frames_x, int frames_y, int width, int height, bool canBeInterrupted);

    virtual ~Animation();

    //Functions
    void play(const float &dt, float mod_percent = 1.f);

    void reset();

    bool isFinished() const;
};


#endif //SFML_ANIMATION_H
