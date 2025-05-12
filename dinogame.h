#ifndef DINOGAME_H
#define DINOGAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "pterodactyl.h"

class DinoGame {
private:
    sf::RenderWindow window;
    
    // Textures
    sf::Texture dinoStandTexture;
    sf::Texture dinoRightUpTexture;
    sf::Texture dinoLeftUpTexture;
    sf::Texture dinoBelowRightUpTexture;
    sf::Texture dinoBelowLeftUpTexture;
    sf::Texture dinoBigEyesTexture;
    sf::Texture groundTexture;
    sf::Texture cactus1Texture;
    sf::Texture cactus2Texture;
    sf::Texture cactus3Texture;
    sf::Texture cactus4Texture;
    sf::Texture cactus5Texture;
    sf::Texture pteroDownTexture;
    sf::Texture pteroUpTexture;
    
    // Sprites
    sf::Sprite dino;
    sf::Sprite ground1;
    sf::Sprite ground2;
    std::vector<sf::Sprite> cacti;
    std::vector<Pterodactyl> pteros;
    
    // Game variables
    float dinoVelocity;
    bool isJumping;
    bool isDucking;
    int score;
    float groundOffset;
    bool gameOver;
    float dinoAnimationTimer;
    int dinoAnimationState;
    float obstacleSpeed;
    float groundScrollSpeed;
    float gameTime;
    
    // Text
    sf::Font font;
    sf::Text scoreText;
    sf::Text gameOverText;
    sf::Text speedText;
    
    // Timers
    sf::Clock obstacleClock;
    sf::Clock pteroSpawnClock;
    sf::Clock scoreClock;
    sf::Clock dinoAnimationClock;
    sf::Clock gameTimeClock;

    void handleEvents();
    void updateGround();
    void updateDino();
    void updateDinoAnimation(float dt);
    void updateCacti();
    void updatePteros(float dt);
    void updateScore();
    void updateGameSpeed();
    void update(float dt);
    void render();
    void resetGame();
    
public:
    DinoGame();
    void run();
};

#endif // DINOGAME_H
