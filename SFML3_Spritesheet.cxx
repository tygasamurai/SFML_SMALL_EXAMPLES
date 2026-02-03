#include <SFML/Graphics.hpp>
#include <vector>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML 3 Spritesheet");
    
    // 1. Load texture
    sf::Texture spritesheetTexture;
    if (!spritesheetTexture.loadFromFile("characters.png")) return -1;

    // 2. Create sprite
    sf::Sprite sprite(spritesheetTexture);

    // 3. Define frames (x, y, width, height)
    std::vector<sf::IntRect> frames = {
        sf::IntRect({0, 0}, {32, 32}),
        sf::IntRect({32, 0}, {32, 32}),
        sf::IntRect({64, 0}, {32, 32})
    };

    int currentFrame = 0;
    sf::Clock animationClock;
    
    while (window.isOpen()) {
        // ... event polling ...

        // 4. Animate: Change frame every 0.2 seconds
        if (animationClock.getElapsedTime().asSeconds() > 0.2f) {
            currentFrame = (currentFrame + 1) % frames.size();
            sprite.setTextureRect(frames[currentFrame]);
            animationClock.restart();
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }
    return 0;
}
