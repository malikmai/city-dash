#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>

// Constants for window dimensions and game mechanics
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float GRAVITY = 0.5f;
const float JUMP_STRENGTH = -10.0f;
const int BLDG_WIDTH = 80;
const int BLDG_HEIGHT = 500;
const int GAP_HEIGHT = 200;

// Ship class definition
class Ship {
public:
    Ship(float x, float y) : position(x, y), velocity(0) {
        shape.setPointCount(3); // Triangle shape for spaceship
        shape.setPoint(0, sf::Vector2f(0, 0));
        shape.setPoint(1, sf::Vector2f(40, 20));
        shape.setPoint(2, sf::Vector2f(0, 40));
        shape.setFillColor(sf::Color::White);
        shape.setPosition(position);
    }

    void update() {
        velocity += GRAVITY;
        position.y += velocity;
        shape.setPosition(position);
    }

    void jump() {
        velocity = JUMP_STRENGTH; // Apply jump strength
    }

    sf::ConvexShape getShape() const {
        return shape;
    }

    sf::FloatRect getBounds() const {
        return shape.getGlobalBounds();
    }

private:
    sf::Vector2f position; // Position of the ship
    float velocity;        // Velocity of the ship
    sf::ConvexShape shape; // Shape representing the ship
};

// BLDG class definition
class BLDG {
public:
    BLDG(float x) {
        topBLDG.setSize(sf::Vector2f(BLDG_WIDTH, BLDG_HEIGHT));
        bottomBLDG.setSize(sf::Vector2f(BLDG_WIDTH, BLDG_HEIGHT));
        topBLDG.setFillColor(sf::Color(54, 1, 63)); // Deep dark purple
        bottomBLDG.setFillColor(sf::Color(54, 1, 63)); // Deep dark purple
        setPosition(x);
    }

    void setPosition(float x) {
        float offsetY = rand() % (WINDOW_HEIGHT - GAP_HEIGHT - 100) + 50;
        topBLDG.setPosition(x, offsetY - BLDG_HEIGHT);
        bottomBLDG.setPosition(x, offsetY + GAP_HEIGHT);
    }

    void update() {
        topBLDG.move(-5.0f, 0);
        bottomBLDG.move(-5.0f, 0);
    }

    bool isOffScreen() const {
        return topBLDG.getPosition().x + BLDG_WIDTH < 0;
    }

    sf::RectangleShape getTopBLDG() const {
        return topBLDG;
    }

    sf::RectangleShape getBottomBLDG() const {
        return bottomBLDG;
    }

    sf::FloatRect getTopBounds() const {
        return topBLDG.getGlobalBounds();
    }

    sf::FloatRect getBottomBounds() const {
        return bottomBLDG.getGlobalBounds();
    }

private:
    sf::RectangleShape topBLDG;
    sf::RectangleShape bottomBLDG;
};

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "City Dash");
    window.setFramerateLimit(60);

    // Load the background image
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("sky.png")) {
        std::cerr << "Failed to load background image!" << std::endl;
        return -1;
    }
    sf::Sprite background(backgroundTexture);

    Ship ship(100, WINDOW_HEIGHT / 2);
    std::vector<BLDG> bldgs;
    bldgs.emplace_back(WINDOW_WIDTH);

    // Load a font for the score and messages
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load font! Using default font." << std::endl;
    }

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);

    int score = 0; // Initialize score
    bool gameOver = false; // Game over flag
    bool gameStarted = false; // Game started flag

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                if (!gameStarted) {
                    gameStarted = true;
                } else if (gameOver) {
                    gameOver = false;
                    ship = Ship(100, WINDOW_HEIGHT / 2);
                    bldgs.clear();
                    bldgs.emplace_back(WINDOW_WIDTH);
                    score = 0;
                } else {
                    ship.jump(); // Make the ship jump when space is pressed
                }
            }
        }

        if (gameStarted && !gameOver) {
            ship.update();

            // Update buildings
            for (auto& bldg : bldgs) {
                bldg.update();
            }

            // Remove off-screen buildings and add new ones
            if (bldgs.front().isOffScreen()) {
                bldgs.erase(bldgs.begin());
                bldgs.emplace_back(WINDOW_WIDTH);
                score++; // Increment score when passing a building
            }

            // Check for collisions
            for (const auto& bldg : bldgs) {
                if (ship.getBounds().intersects(bldg.getTopBounds()) || ship.getBounds().intersects(bldg.getBottomBounds())) {
                    gameOver = true;
                }
            }

            // Check if the ship hits the top or bottom of the window
            if (ship.getBounds().top + ship.getBounds().height > WINDOW_HEIGHT || ship.getBounds().top < 0) {
                gameOver = true;
            }

            scoreText.setString("Score: " + std::to_string(score));
        }

        window.clear();
        window.draw(background);
        window.draw(ship.getShape());
        for (const auto& bldg : bldgs) {
            window.draw(bldg.getTopBLDG());
            window.draw(bldg.getBottomBLDG());
        }
        if (gameStarted) {
            window.draw(scoreText);
        }
        window.display();
    }

    return 0;
}
