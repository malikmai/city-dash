#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>

// Constants for window dimensions and game mechanics
const int WINDOW_WIDTH = 800; // Width of the game window
const int WINDOW_HEIGHT = 600; // Height of the game window
const float GRAVITY = 0.5f; // Gravity affecting the ship
const float JUMP_STRENGTH = -10.0f; // Strength of the ship's jump
const int BLDG_WIDTH = 80; // Width of the buildings
const int BLDG_HEIGHT = 500; // Height of the buildings
const int GAP_HEIGHT = 200; // Gap height between top and bottom buildings

// Ship class definition
class Ship {
public:
    Ship(float x, float y) : position(x, y), velocity(0) {
        shape.setPointCount(3); // Triangle shape for spaceship
        shape.setPoint(0, sf::Vector2f(0, 0));
        shape.setPoint(1, sf::Vector2f(40, 20));
        shape.setPoint(2, sf::Vector2f(0, 40));
        shape.setFillColor(sf::Color::White); // Color of the ship
        shape.setPosition(position); // Initial position of the ship
    }

    void update() {
        velocity += GRAVITY; // Apply gravity to the ship's velocity
        position.y += velocity; // Update ship's vertical position
        shape.setPosition(position); // Update ship's shape position
    }

    void jump() {
        velocity = JUMP_STRENGTH; // Apply jump strength to the ship's velocity
    }

    sf::ConvexShape getShape() const {
        return shape; // Return the shape of the ship
    }

    sf::FloatRect getBounds() const {
        return shape.getGlobalBounds(); // Return the bounding box of the ship
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
        topBLDG.setSize(sf::Vector2f(BLDG_WIDTH, BLDG_HEIGHT)); // Set size of the top building
        bottomBLDG.setSize(sf::Vector2f(BLDG_WIDTH, BLDG_HEIGHT)); // Set size of the bottom building
        topBLDG.setFillColor(sf::Color(54, 1, 63)); // Color of the top building
        bottomBLDG.setFillColor(sf::Color(54, 1, 63)); // Color of the bottom building
        setPosition(x); // Set initial position of the buildings
    }

    void setPosition(float x) {
        float offsetY = rand() % (WINDOW_HEIGHT - GAP_HEIGHT - 100) + 50; // Randomly offset the buildings vertically
        topBLDG.setPosition(x, offsetY - BLDG_HEIGHT); // Position the top building
        bottomBLDG.setPosition(x, offsetY + GAP_HEIGHT); // Position the bottom building
    }

    void update() {
        topBLDG.move(-5.0f, 0); // Move top building to the left
        bottomBLDG.move(-5.0f, 0); // Move bottom building to the left
    }

    bool isOffScreen() const {
        return topBLDG.getPosition().x + BLDG_WIDTH < 0; // Check if the building is off the screen
    }

    sf::RectangleShape getTopBLDG() const {
        return topBLDG; // Return the top building
    }

    sf::RectangleShape getBottomBLDG() const {
        return bottomBLDG; // Return the bottom building
    }

    sf::FloatRect getTopBounds() const {
        return topBLDG.getGlobalBounds(); // Return the bounding box of the top building
    }

    sf::FloatRect getBottomBounds() const {
        return bottomBLDG.getGlobalBounds(); // Return the bounding box of the bottom building
    }

private:
    sf::RectangleShape topBLDG; // Shape representing the top building
    sf::RectangleShape bottomBLDG; // Shape representing the bottom building
};

int main() {
    srand(static_cast<unsigned int>(time(nullptr))); // Seed the random number generator

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "City Dash"); // Create the game window
    window.setFramerateLimit(60); // Set the framerate limit to 60 FPS

    // Load the background image
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("sky.png")) {
        std::cerr << "Failed to load background image!" << std::endl; // Error handling if the background image fails to load
        return -1;
    }
    sf::Sprite background(backgroundTexture); // Create a sprite for the background image

    Ship ship(100, WINDOW_HEIGHT / 2); // Create the ship at the starting position
    std::vector<BLDG> bldgs; // Vector to store the buildings
    bldgs.emplace_back(WINDOW_WIDTH); // Add the first building at the edge of the screen

    // Load a font for the score and messages
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load font! Using default font." << std::endl; // Error handling if the font fails to load
    }

    sf::Text startText;
    startText.setFont(font); // Set the font for the start text
    startText.setCharacterSize(36); // Set the character size
    startText.setFillColor(sf::Color::White); // Set the text color
    startText.setString("Press SPACE to Start"); // Set the start message
    startText.setPosition(WINDOW_WIDTH / 2 - startText.getGlobalBounds().width / 2, WINDOW_HEIGHT / 2 - startText.getGlobalBounds().height / 2); // Center the start text

    sf::Text scoreText;
    scoreText.setFont(font); // Set the font for the score text
    scoreText.setCharacterSize(24); // Set the character size
    scoreText.setFillColor(sf::Color::White); // Set the text color
    scoreText.setPosition(10, 10); // Set the position of the score text

    sf::Text gameOverText;
    gameOverText.setFont(font); // Set the font for the game over text
    gameOverText.setCharacterSize(36); // Set the character size
    gameOverText.setFillColor(sf::Color::White); // Set the text color
    gameOverText.setString("Game Over! You've Crashed\nPress SPACE to Play Again"); // Set the game over message
    gameOverText.setPosition(WINDOW_WIDTH / 2 - gameOverText.getGlobalBounds().width / 2, WINDOW_HEIGHT / 2 - gameOverText.getGlobalBounds().height / 2); // Center the game over text

    int score = 0; // Initialize score
    bool gameOver = false; // Game over flag
    bool gameStarted = false; // Game started flag

    // Main game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close(); // Close the window if the close event is triggered
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                if (!gameStarted) {
                    gameStarted = true; // Start the game when space is pressed
                } else if (gameOver) {
                    gameOver = false; // Reset the game if it's over and space is pressed
                    ship = Ship(100, WINDOW_HEIGHT / 2); // Reset the ship
                    bldgs.clear(); // Clear the buildings
                    bldgs.emplace_back(WINDOW_WIDTH); // Add the first building
                    score = 0; // Reset the score
                } else {
                    ship.jump(); // Make the ship jump when space is pressed during the game
                }
            }
        }

        if (gameStarted && !gameOver) {
            ship.update(); // Update the ship's position

            // Update buildings
            for (auto& bldg : bldgs) {
                bldg.update(); // Update each building's position
            }

            // Remove off-screen buildings and add new ones
            if (bldgs.front().isOffScreen()) {
                bldgs.erase(bldgs.begin()); // Remove the first building if it's off-screen
                bldgs.emplace_back(WINDOW_WIDTH); // Add a new building at the edge of the screen
                score++; // Increment score when passing a building
            }

            // Check for collisions
            for (const auto& bldg : bldgs) {
                if (ship.getBounds().intersects(bldg.getTopBounds()) || ship.getBounds().intersects(bldg.getBottomBounds())) {
                    gameOver = true; // Set game over flag if the ship collides with a building
                }
            }

            // Check if the ship hits the top or bottom of the window
            if (ship.getBounds().top + ship.getBounds().height > WINDOW_HEIGHT || ship.getBounds().top < 0) {
                gameOver = true; // Set game over flag if the ship goes out of the window bounds
            }

            scoreText.setString("Score: " + std::to_string(score)); // Update the score text
        }

        window.clear(); // Clear the window
        window.draw(background); // Draw the background
        window.draw(ship.getShape()); // Draw the ship
        for (const auto& bldg : bldgs) {
            window.draw(bldg.getTopBLDG()); // Draw the top building
            window.draw(bldg.getBottomBLDG()); // Draw the bottom building
        }
        if (gameStarted) {
            if (gameOver) {
                window.draw(gameOverText); // Draw the game over text if the game is over
            } else {
                window.draw(scoreText); // Draw the score text during the game
            }
        } else {
            window.draw(startText); // Draw the start text before the game starts
        }
        window.display(); // Display the contents of the window
    }

    return 0; // Return success
}
