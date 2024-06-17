#include <SFML/Graphics.hpp>
#include <iostream>

// Constants for window dimensions and gravity
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float GRAVITY = 0.5f;

// Ship class definition
class Ship {
public:
    // Constructor
    Ship(float x, float y) : position(x, y), velocity(0) {
        shape.setPointCount(3); // Triangle shape for spaceship
        shape.setPoint(0, sf::Vector2f(0, 0));
        shape.setPoint(1, sf::Vector2f(40, 20));
        shape.setPoint(2, sf::Vector2f(0, 40));
        shape.setFillColor(sf::Color::White);
        shape.setPosition(position);
    }

    // Update the ship's position based on gravity
    void update() {
        velocity += GRAVITY;
        position.y += velocity;
        shape.setPosition(position);
    }

    // Get the shape to draw
    sf::ConvexShape getShape() const {
        return shape;
    }

private:
    sf::Vector2f position; // Position of the ship
    float velocity;        // Velocity of the ship
    sf::ConvexShape shape; // Shape representing the ship
};

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "City Dash");
    window.setFramerateLimit(60);

    // Create a ship instance
    Ship ship(100, WINDOW_HEIGHT / 2);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Update the ship
        ship.update();

        window.clear();
        window.draw(ship.getShape());
        window.display();
    }

    return 0;
}
