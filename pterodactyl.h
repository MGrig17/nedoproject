#ifndef PTERODACTYL_H
#define PTERODACTYL_H

#include <SFML/Graphics.hpp>

class Pterodactyl {
public:
    sf::Sprite sprite;
    bool active;
    float animationTimer;
    bool currentFrame;
    
    Pterodactyl(const sf::Texture& downTex, const sf::Texture& upTex);
    void update(float dt, float speed);
    void spawn(float x, float y, float speed);

private:
    sf::Texture downTexture;
    sf::Texture upTexture;
};

#endif // PTERODACTYL_H
