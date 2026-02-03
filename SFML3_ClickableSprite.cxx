#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML 3 Inventory");

    // Inventory settings
    const int rows = 4;
    const int cols = 5;
    const float slotSize = 60.f;
    const float padding = 10.f;

    // Create slot backgrounds
    std::vector<sf::RectangleShape> slots;
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            sf::RectangleShape slot({slotSize, slotSize});
            slot.setFillColor(sf::Color(100, 100, 100)); // Grey background
            slot.setOutlineThickness(2.f);
            slot.setOutlineColor(sf::Color::White);
            slot.setPosition({x * (slotSize + padding) + 50.f, y * (slotSize + padding) + 50.f});
            slots.push_back(slot);
        }
    }

    while (window.isOpen()) {
        // SFML 3 modern event loop using std::optional
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            // Check for mouse clicks using the new getIf syntax
            if (const auto* click = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (click->button == sf::Mouse::Button::Left) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(click->position);
                    
                    for (size_t i = 0; i < slots.size(); ++i) {
                        if (slots[i].getGlobalBounds().contains(mousePos)) {
                            slots[i].setFillColor(sf::Color::Green); // Highlight selected slot
                        }
                    }
                }
            }
        }

        window.clear();
        for (const auto& slot : slots) {
            window.draw(slot);
        }
        window.display();
    }

    return 0;
}
