#include "pterodactyl.h"

Pterodactyl::Pterodactyl(const sf::Texture& downTex, const sf::Texture& upTex) 
    : downTexture(downTex), upTexture(upTex), active(false), animationTimer(0), currentFrame(false) {
    sprite.setTexture(downTexture);
    sprite.setScale(0.5f, 0.5f);
}

void Pterodactyl::update(float dt, float speed) {
    if (!active) return;
    
    animationTimer += dt;
    if (animationTimer >= 0.2f) { // PTERO_ANIMATION_SPEED
        animationTimer = 0;
        currentFrame = !currentFrame;
        sprite.setTexture(currentFrame ? upTexture : downTexture);
    }
    
    sprite.move(-speed, 0);
    
    if (sprite.getPosition().x < -100) {
        active = false;
    }
}

void Pterodactyl::spawn(float x, float y, float speed) {
    sprite.setPosition(x, y + 25);
    active = true;
    animationTimer = 0;
    currentFrame = false;
    sprite.setTexture(downTexture);
}
