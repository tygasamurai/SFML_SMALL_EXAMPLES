#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class KeyboardMenu {
public:
    KeyboardMenu(const sf::Font& font, const std::vector<std::string>& options) {
        for (std::size_t i = 0; i < options.size(); ++i) {
            sf::Text text(font, options[i], 30);
            text.setPosition({ 300.f, 100.f + (i * 50.f) });
            items.push_back(std::move(text));
        }
        updateSelection();
    }

    void moveUp() {
        if (selectedIndex > 0) {
            selectedIndex--;
            updateSelection();
        }
    }

    void moveDown() {
        if (selectedIndex + 1 < items.size()) {
            selectedIndex++;
            updateSelection();
        }
    }

    int getSelectedIndex() const { return selectedIndex; }

    void draw(sf::RenderWindow& window) {
        for (const auto& item : items) window.draw(item);
    }

private:
    std::vector<sf::Text> items;
    int selectedIndex = 0;

    void updateSelection() {
        for (int i = 0; i < items.size(); ++i) {
            items[i].setFillColor(i == selectedIndex ? sf::Color::Yellow : sf::Color::White);
        }
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "SFML 3 Keyboard Menu");
    sf::Font font;
    if (!font.openFromFile("arial.ttf")) return -1;

    KeyboardMenu menu(font, { "Start Game", "Options", "Exit" });

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();

            // Handle KeyPress events in SFML 3 style
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Up) menu.moveUp();
                if (keyPressed->code == sf::Keyboard::Key::Down) menu.moveDown();
                if (keyPressed->code == sf::Keyboard::Key::Enter) {
                    if (menu.getSelectedIndex() == 2) window.close(); // Exit logic
                }
            }
        }

        window.clear();
        menu.draw(window);
        window.display();
    }
}