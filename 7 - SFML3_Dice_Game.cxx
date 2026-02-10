#include <SFML/Graphics.hpp>
#include <random> // Modern C++ random library
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML 3 Dice Game");

    // Modern random number generation
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(1, 6);

    // Load assets (you would set up a Dice class with a sprite here)
    // ... setup text or sprites to display the dice ...

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            // Check for spacebar press to roll dice
            if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                if (keyEvent->scancode == sf::Keyboard::Scan::Space) {
                    int roll1 = dist(mt);
                    int roll2 = dist(mt);
                    std::cout << "Rolled: " << roll1 << ", " << roll2 << std::endl;
                    // Update your sf::Sprite or sf::Text objects here to show the results
                }
            }
        }

        window.clear(sf::Color::White);
        // ... draw dice sprites/text here ...
        window.display();
    }
    return 0;
}
