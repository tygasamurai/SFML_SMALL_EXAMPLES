#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class DialogueBox {
public:
    DialogueBox(const sf::Font& font, const sf::Vector2f& size) {
        // Modern SFML 3.0 uses sf::Vector2f for sizes and positions
        background.setSize(size);
        background.setFillColor(sf::Color(0, 0, 0, 200)); // Semi-transparent black
        background.setOutlineColor(sf::Color::White);
        background.setOutlineThickness(2.f);

        text.setFont(font);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);
    }

    void setMessage(const std::string& fullString) {
        fullMessage = fullString;
        currentDisplay = "";
        charIndex = 0;
        clock.restart();
    }

    void update() {
        // Typewriter effect: Add one character every 50ms
        if (charIndex < fullMessage.size() && clock.getElapsedTime().asMilliseconds() > 50) {
            currentDisplay += fullMessage[charIndex];
            text.setString(currentDisplay);
            charIndex++;
            clock.restart();
        }
    }

    void setPosition(const sf::Vector2f& pos) {
        background.setPosition(pos);
        text.setPosition({pos.x + 10.f, pos.y + 10.f}); // Padding
    }

    void draw(sf::RenderWindow& window) {
        window.draw(background);
        window.draw(text);
    }

private:
    sf::RectangleShape background;
    sf::Text text;
    sf::Clock clock;
    std::string fullMessage;
    std::string currentDisplay;
    size_t charIndex = 0;
};

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "RPG Dialogue Example");
    
    sf::Font font("arial.ttf"); // Ensure you have a font file in your directory
    DialogueBox dialog(font, {780.f, 150.f});
    dialog.setPosition({10.f, 440.f});
    dialog.setMessage("Welcome, traveler! Are you ready for your quest?");

    while (window.isOpen()) {
        // Modern SFML 3.0 event loop
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        dialog.update();

        window.clear();
        dialog.draw(window);
        window.display();
    }
}
