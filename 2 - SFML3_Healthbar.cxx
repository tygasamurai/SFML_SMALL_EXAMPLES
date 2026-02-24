#include <SFML/Graphics.hpp>
#include <algorithm> // For std::max

class HealthBar {
private:
    sf::RectangleShape background;
    sf::RectangleShape bar;
    float maxHealth;
    float width = 200.f;
    float height = 20.f;

public:
    HealthBar(float maxHp, sf::Vector2f position) : maxHealth(maxHp) {
        // Setup background (gray)
        background.setSize(sf::Vector2f(width, height));
        background.setFillColor(sf::Color(100, 100, 100)); // Dark Gray

        // Setup bar (red)
        bar.setSize(sf::Vector2f(width, height));
        bar.setFillColor(sf::Color::Red);

                // Position the health bar (e.g., above player)
        background.setPosition(position);
        bar.setPosition(position);
    }

    void setPosition(sf::Vector2f position ) 
    {  
        // Position the health bar (e.g., above player)
        background.setPosition(position);
        bar.setPosition(position);
    }

    void update(float currentHealth) {
        // Ensure health doesn't go below 0
        float healthPercentage = std::max(0.f, currentHealth) / maxHealth;
        
        // Update bar width based on health percentage
        bar.setSize(sf::Vector2f(width * healthPercentage, height));
    }

    void draw(sf::RenderWindow& window) {
        window.draw(background);
        window.draw(bar);
    }
};

HealthBar hb = HealthBar(100, {200.f, 50.f});


