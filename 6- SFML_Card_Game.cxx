#include <SFML/Graphics.hpp>

class Card {
public:
    // Define card dimensions based on your sprite sheet
    static constexpr int WIDTH = 64;
    static constexpr int HEIGHT = 96;

    Card(sf::Texture& atlas, int suit, int value) {
        sprite.setTexture(atlas);
        // Modern SFML 3.0 Rect uses sf::Vector2i for position and size
        sf::IntRect textureRect({value * WIDTH, suit * HEIGHT}, {WIDTH, HEIGHT});
        sprite.setTextureRect(textureRect);
    }

    void setPosition(sf::Vector2f pos) { sprite.setPosition(pos); }
    void draw(sf::RenderWindow& window) { window.draw(sprite); }

    // Check for clicks (useful for playing/moving cards)
    bool isClicked(sf::Vector2f mousePos) const {
        return sprite.getGlobalBounds().contains(mousePos);
    }

private:
    sf::Sprite sprite;
};

int main() {
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "SFML 3 Card Game");
    
    sf::Texture atlas("cards_sheet.png");
    Card aceOfSpades(atlas, 0, 0); // Assuming 0,0 is the first card in your sheet
    aceOfSpades.setPosition({100.f, 100.f});

    while (window.isOpen()) {
        // SFML 3.0 Modern Event Handling
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            // Detect Click on Card
            if (const auto* mouseBtn = event->getIf<sf::Event::MouseButtonPressed>()) {
                sf::Vector2f mousePos = window.mapPixelToCoords(mouseBtn->position);
                if (aceOfSpades.isClicked(mousePos)) {
                    // Logic for card selection or play
                }
            }
        }

        window.clear(sf::Color(0, 100, 0)); // Classic card table green
        aceOfSpades.draw(window);
        window.display();
    }
}
