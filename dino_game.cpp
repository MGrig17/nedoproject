#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 300;
const float GRAVITY = 0.5f;
const float JUMP_FORCE = -12.0f;
const float INITIAL_OBSTACLE_SPEED = 5.0f;
const float INITIAL_GROUND_SPEED = 4.0f;
const float DINO_ANIMATION_SPEED = 0.15f;
const float PTERO_ANIMATION_SPEED = 0.2f;
const int GROUND_OFFSET_Y = -10;
const float SPEED_INCREASE_FACTOR = 0.2f;
const float SPEED_INCREASE_INTERVAL = 30.0f;
const float PTERO_SPAWN_INTERVAL = 5.0f;
const float CACTUS_SPAWN_INTERVAL = 1.5f;

class Pterodactyl {
public:
    sf::Sprite sprite;
    bool active;
    float animationTimer;
    bool currentFrame;
    
    Pterodactyl(const sf::Texture& downTex, const sf::Texture& upTex) 
        : downTexture(downTex), upTexture(upTex), active(false), animationTimer(0), currentFrame(false) {
        sprite.setTexture(downTexture);
        sprite.setScale(0.5f, 0.5f);
    }
    
    void update(float dt, float speed) {
        if (!active) return;
        
        animationTimer += dt;
        if (animationTimer >= PTERO_ANIMATION_SPEED) {
            animationTimer = 0;
            currentFrame = !currentFrame;
            sprite.setTexture(currentFrame ? upTexture : downTexture);
        }
        
        sprite.move(-speed, 0);
        
        if (sprite.getPosition().x < -100) {
            active = false;
        }
    }
    
    void spawn(float x, float y, float speed) {
        sprite.setPosition(x, y + 25);
        active = true;
        animationTimer = 0;
        currentFrame = false;
        sprite.setTexture(downTexture);
    }

private:
    sf::Texture downTexture;
    sf::Texture upTexture;
};

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

    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    if (!isJumping && !gameOver) {
                        dinoVelocity = JUMP_FORCE;
                        isJumping = true;
                    } else if (gameOver) {
                        resetGame();
                    }
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    isDucking = true;
                }
            }
            else if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Down) {
                    isDucking = false;
                }
            }
        }
    }
    
    void updateGround() {
        if (gameOver) return;
        
        groundOffset += groundScrollSpeed;
        if (groundOffset > 1200) {
            groundOffset = 0;
        }
        
        ground1.setPosition(-groundOffset, WINDOW_HEIGHT - 12 + GROUND_OFFSET_Y);
        ground2.setPosition(1200 - groundOffset, WINDOW_HEIGHT - 12 + GROUND_OFFSET_Y);
    }
    
    void updateDino() {
        if (gameOver) return;
        
        dinoVelocity += GRAVITY;
        dino.move(0, dinoVelocity);
        
        float groundLevel = WINDOW_HEIGHT - 7 - (isDucking ? 28 : 45) + GROUND_OFFSET_Y;
        if (dino.getPosition().y >= groundLevel) {
            dino.setPosition(dino.getPosition().x, groundLevel);
            dinoVelocity = 0;
            isJumping = false;
        }
    }
    
    void updateDinoAnimation(float dt) {
        if (gameOver) return;
        
        dinoAnimationTimer += dt;
        
        if (dinoAnimationTimer >= DINO_ANIMATION_SPEED) {
            dinoAnimationTimer = 0;
            
            if (isDucking) {
                dinoAnimationState = (dinoAnimationState + 1) % 2;
                
                switch (dinoAnimationState) {
                    case 0: dino.setTexture(dinoBelowRightUpTexture); break;
                    case 1: dino.setTexture(dinoBelowLeftUpTexture); break;
                }
            }
            else if (!isJumping) {
                dinoAnimationState = (dinoAnimationState + 1) % 3;
                
                switch (dinoAnimationState) {
                    case 0: dino.setTexture(dinoStandTexture); break;
                    case 1: dino.setTexture(dinoRightUpTexture); break;
                    case 2: dino.setTexture(dinoLeftUpTexture); break;
                }
            } else {
                dino.setTexture(dinoStandTexture);
            }
        }
    }
    
    void updateCacti() {
        if (gameOver) return;
        
        if (obstacleClock.getElapsedTime().asSeconds() > CACTUS_SPAWN_INTERVAL) {
            if (rand() % 100 < 30) {
                sf::Sprite cactus;
                int cactusType = rand() % 5;
                switch (cactusType) {
                    case 0: cactus.setTexture(cactus1Texture); break;
                    case 1: cactus.setTexture(cactus2Texture); break;
                    case 2: cactus.setTexture(cactus3Texture); break;
                    case 3: cactus.setTexture(cactus4Texture); break;
                    case 4: cactus.setTexture(cactus5Texture); break;
                }
                cactus.setPosition(WINDOW_WIDTH, WINDOW_HEIGHT - 7 - cactus.getGlobalBounds().height + GROUND_OFFSET_Y);
                cacti.push_back(cactus);
            }
            obstacleClock.restart();
        }
        
        for (size_t i = 0; i < cacti.size(); ) {
            cacti[i].move(-obstacleSpeed, 0);
            
            if (cacti[i].getPosition().x < -50) {
                cacti.erase(cacti.begin() + i);
            } else {
                if (dino.getGlobalBounds().intersects(cacti[i].getGlobalBounds())) {
                    gameOver = true;
                    dino.setTexture(dinoBigEyesTexture);
                }
                i++;
            }
        }
    }
    
    void updatePteros(float dt) {
        if (gameOver) return;
        
        for (auto& ptero : pteros) {
            ptero.update(dt, obstacleSpeed);
            
            if (ptero.active && dino.getGlobalBounds().intersects(ptero.sprite.getGlobalBounds())) {
                gameOver = true;
                dino.setTexture(dinoBigEyesTexture);
            }
        }
        
        if (pteroSpawnClock.getElapsedTime().asSeconds() > PTERO_SPAWN_INTERVAL) {
            if (rand() % 100 < 20) {
                float height = WINDOW_HEIGHT - 12 - 50 + GROUND_OFFSET_Y - (rand() % 100);
                
                for (auto& ptero : pteros) {
                    if (!ptero.active) {
                        ptero.spawn(WINDOW_WIDTH, height, obstacleSpeed);
                        break;
                    }
                }
            }
            pteroSpawnClock.restart();
        }
    }
    
    void updateScore() {
        if (gameOver) return;
        
        if (scoreClock.getElapsedTime().asSeconds() > 0.1f) {
            score += 1;
            scoreClock.restart();
        }
        scoreText.setString("Score: " + std::to_string(score));
    }
    
    void updateGameSpeed() {
        if (gameOver) return;
        
        gameTime = gameTimeClock.getElapsedTime().asSeconds();
        int speedLevel = static_cast<int>(gameTime / SPEED_INCREASE_INTERVAL);
        obstacleSpeed = INITIAL_OBSTACLE_SPEED + speedLevel * SPEED_INCREASE_FACTOR;
        groundScrollSpeed = INITIAL_GROUND_SPEED + speedLevel * SPEED_INCREASE_FACTOR;
        
        speedText.setString("Speed: x" + std::to_string(1 + speedLevel * SPEED_INCREASE_FACTOR / INITIAL_OBSTACLE_SPEED).substr(0, 4));
    }
    
    void update(float dt) {
        updateGround();
        updateDino();
        updateDinoAnimation(dt);
        updateCacti();
        updatePteros(dt);
        updateScore();
        updateGameSpeed();
    }
    
    void render() {
        window.clear(sf::Color::White);
        
        window.draw(ground1);
        window.draw(ground2);
        
        for (const auto& cactus : cacti) {
            window.draw(cactus);
        }
        
        for (const auto& ptero : pteros) {
            if (ptero.active) {
                window.draw(ptero.sprite);
            }
        }
        
        window.draw(dino);
        window.draw(scoreText);
        window.draw(speedText);
        
        if (gameOver) {
            window.draw(gameOverText);
        }
        
        window.display();
    }
    
    void resetGame() {
        dino.setTexture(dinoStandTexture);
        dino.setPosition(100, WINDOW_HEIGHT - 12 - 45 + GROUND_OFFSET_Y);
        dinoVelocity = 0;
        isJumping = false;
        isDucking = false;
        cacti.clear();
        score = 0;
        groundOffset = 0;
        gameOver = false;
        dinoAnimationState = 0;
        dinoAnimationTimer = 0;
        obstacleSpeed = INITIAL_OBSTACLE_SPEED;
        groundScrollSpeed = INITIAL_GROUND_SPEED;
        gameTime = 0;
        
        for (auto& ptero : pteros) {
            ptero.active = false;
        }
        
        obstacleClock.restart();
        pteroSpawnClock.restart();
        scoreClock.restart();
        dinoAnimationClock.restart();
        gameTimeClock.restart();
    }
    
public:
    DinoGame() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Dino Game") {
        window.setFramerateLimit(60);
        
        // Load textures
        if (!dinoStandTexture.loadFromFile("Dino-stand.png") ||
            !dinoRightUpTexture.loadFromFile("Dino-right-up.png") ||
            !dinoLeftUpTexture.loadFromFile("Dino-left-up.png") ||
            !dinoBelowRightUpTexture.loadFromFile("Dino-below-right-up.png") ||
            !dinoBelowLeftUpTexture.loadFromFile("Dino-below-left-up.png") ||
            !dinoBigEyesTexture.loadFromFile("Dino-big-eyes.png") ||
            !groundTexture.loadFromFile("ground.png") ||
            !cactus1Texture.loadFromFile("Cactus-1.png") ||
            !cactus2Texture.loadFromFile("Cactus-2.png") ||
            !cactus3Texture.loadFromFile("Cactus-3.png") ||
            !cactus4Texture.loadFromFile("Cactus-4.png") ||
            !cactus5Texture.loadFromFile("Cactus-5.png") ||
            !pteroDownTexture.loadFromFile("Ptero-down.png") ||
            !pteroUpTexture.loadFromFile("Ptero-up.png")) {
            std::cerr << "Failed to load textures!" << std::endl;
            exit(1);
        }
        
        if (!font.loadFromFile("arial.ttf")) {
            font = sf::Font();
        }
        
        // Setup sprites
        dino.setTexture(dinoStandTexture);
        
        ground1.setTexture(groundTexture);
        ground2.setTexture(groundTexture);
        
        // Create pterodactyl pool
        for (int i = 0; i < 5; ++i) {
            pteros.emplace_back(pteroDownTexture, pteroUpTexture);
        }
        
        // Setup text
        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::Black);
        scoreText.setPosition(10, 10);
        
        speedText.setFont(font);
        speedText.setCharacterSize(24);
        speedText.setFillColor(sf::Color::Black);
        speedText.setPosition(10, 40);
        
        gameOverText.setFont(font);
        gameOverText.setString("Game Over! Press SPACE to restart");
        gameOverText.setCharacterSize(30);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setPosition(WINDOW_WIDTH/2 - 200, WINDOW_HEIGHT/2 - 50);
        
        resetGame();
        srand(time(nullptr));
    }
    
    void run() {
        sf::Clock frameClock;
        while (window.isOpen()) {
            float dt = frameClock.restart().asSeconds();
            handleEvents();
            update(dt);
            render();
        }
    }
};

int main() {
    DinoGame game;
    game.run();
    return 0;
}
