#include <SFML/Graphics.hpp>
#include <iostream>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float GRAVITY = 0.5f;
const float JUMP_STRENGTH = -10.0f; // Strength of the jump

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

private:
    sf::Vector2f position;
    float velocity;
    sf::ConvexShape shape;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "City Dash");
    window.setFramerateLimit(60);

    Ship ship(100, WINDOW_HEIGHT / 2);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                ship.jump(); // Make the ship jump when space is pressed
            }
        }

        ship.update();

        window.clear();
        window.draw(ship.getShape());
        window.display();
    }

    return 0;
}
