#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <string>


/*
The example below integrates networking directly into the SFML game loop. 
The Non-Blocking Multi-Player Client
This client connects to the server and uses setBlocking(false) to ensure the game remains responsive.
*/
int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML 3 Client");
    sf::TcpSocket socket;

    // 1. Connect to the server (e.g., LocalHost or specific IP)
    sf::IpAddress serverIp = sf::IpAddress::LocalHost; 
    if (socket.connect(serverIp, 53000) != sf::Socket::Status::Done) {
        std::cerr << "Could not connect to server!\n";
        return -1;
    }

    // 2. IMPORTANT: Set to non-blocking so networking doesn't freeze the loop
    socket.setBlocking(false); 

    std::string chatLog = "Connected to server!\n";

    while (window.isOpen()) {
        // --- PROCESS INPUTS ---
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            // Example: Press 'S' to send a test message
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::S) {
                    sf::Packet packet;
                    packet << "Hello from a player!";
                    socket.send(packet);
                }
            }
        }

        // --- RECEIVE UPDATES (Non-Blocking) ---
        sf::Packet packet;
        sf::Socket::Status status = socket.receive(packet);
        if (status == sf::Socket::Status::Done) {
            std::string receivedMsg;
            if (packet >> receivedMsg) {
                std::cout << "Server Broadcast: " << receivedMsg << "\n";
            }
        } else if (status == sf::Socket::Status::Disconnected) {
            std::cout << "Lost connection to server.\n";
            window.close();
        }

        // --- RENDER ---
        window.clear();
        // Draw game entities here...
        window.display();
    }

    return 0;
}

/*
Best Practices Explained
Non-Blocking Mode: 
By calling socket.setBlocking(false), the receive() function returns immediately 
with sf::Socket::Status::NotReady if no new data has arrived, allowing the frame to finish rendering.
Packet Chaining: 
For complex data, you can overload the << and >> operators 
for custom structs (like PlayerPosition) to send multiple variables in one efficient packet.
Error Handling: 
Check sf::Socket::Status::Disconnected specifically to 
handle server shutdowns gracefully instead of the client just hanging or crashing.
Fixed-Size Types: 
When sending integers, use sf::Uint32 or sf::Int16 instead of 
standard int to ensure data remains consistent across different operating systems. 

*/