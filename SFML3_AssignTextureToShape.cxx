#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode({400, 400}), "SFML3 Texture Shape");

    // 1. Load the texture
    sf::Texture texture;
    if (!texture.loadFromFile("image.png")) {
        return -1; // Error handling
    }

    // 2. Create the shape
    sf::RectangleShape shape({200.f, 150.f});
    shape.setPosition({100.f, 100.f});

    // 3. Assign the texture
    shape.setTexture(&texture);

    // Optional: Set a specific part of the texture (Rect: top, left, width, height)
    // shape.setTextureRect(sf::IntRect({0, 0}, {50, 50}));

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.draw(shape); // Draw the textured shape
        window.display();
    }

    return 0;
}
