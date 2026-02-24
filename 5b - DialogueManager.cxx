#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <optional>

struct DialogueBlock {
    std::string speaker;
    std::vector<std::string> lines;
};

class DialogueManager {
public:
    // 1. Storage: Load all game dialogue into this map once at start
    void loadDialogue(std::string id, DialogueBlock block) {
        m_library[id] = std::move(block);
    }

    // 2. State: Trigger a dialogue by its ID to add it to the active queue
    void trigger(const std::string& id) {
        if (m_library.contains(id)) {
            m_activeQueue.push_back(m_library[id]);
            if (!m_active) startNextBlock();
        }
    }

    void advance() {
        if (!m_active) return;
        m_lineIndex++;
        
        if (m_lineIndex >= m_activeQueue.front().lines.size()) {
            m_activeQueue.pop_front(); // Finish this block
            startNextBlock();
        }
    }

    bool isActive() const { return m_active; }

    void draw(sf::RenderWindow& window, const sf::Font& font) {
        if (!m_active) return;

        const auto& current = m_activeQueue.front();
        
        // SFML 3 UI rendering (using Vector2f for positions/sizes)
        sf::RectangleShape box({760.f, 150.f});
        box.setPosition({20.f, 430.f});
        box.setFillColor(sf::Color(30, 30, 30, 240));

        sf::Text text(font, current.speaker + ": " + current.lines[m_lineIndex], 22);
        text.setPosition({40.f, 450.f});
        
        window.draw(box);
        window.draw(text);
    }

private:
    void startNextBlock() {
        m_lineIndex = 0;
        m_active = !m_activeQueue.empty();
    }

    std::map<std::string, DialogueBlock> m_library; // The "Database"
    std::deque<DialogueBlock> m_activeQueue;        // The "Playing Now"
    size_t m_lineIndex = 0;
    bool m_active = false;
};

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML 3 Map Dialogue");
    sf::Font font("arial.ttf"); 
    DialogueManager dm;

    // Load data (usually done from a JSON/file)
    dm.loadDialogue("intro", {"Narrator", {"Welcome to the realm.", "It is cold here."}});
    dm.loadDialogue("shop", {"Merchant", {"Need some potions?", "They're 50 gold each."}});

    // Trigger specific dialogues when needed
    dm.trigger("intro");

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) { // SFML 3 Optional Event
            if (event->is<sf::Event::Closed>()) window.close();
            if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Space) dm.advance();
                if (key->code == sf::Keyboard::Key::S) dm.trigger("shop"); // Press S to talk to merchant
            }
        }
        window.clear();
        dm.draw(window, font);
        window.display();
    }
}
