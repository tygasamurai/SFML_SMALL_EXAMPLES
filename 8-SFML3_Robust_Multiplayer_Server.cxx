#include <SFML/Network.hpp>
#include <iostream>
#include <list>
#include <string>


/*
To create a stable multi-player game server, we must address two main "crash-prone" areas: 
Memory Stability (ensuring sockets don't move in memory) and 
Resource Management (gracefully handling unexpected disconnections).
The example below combines the Selector logic with a Broadcast function, 
allowing one player's message to be sent to everyone else.
The Robust Multi-Player Server
This code uses std::list for sockets because, unlike std::vector, 
adding or removing elements in a list does not move the other elements in memory, 
which keeps the sf::SocketSelector references valid.

*/
// Helper function to send a packet to every connected client
void broadcast(const std::string& text, std::list<sf::TcpSocket>& clients) {
    sf::Packet packet;
    packet << text;
    for (auto& client : clients) {
        client.send(packet);
    }
}

int main() {
    sf::TcpListener listener;
    const unsigned short port = 53000;

    // Bind the listener to the port
    if (listener.listen(port) != sf::Socket::Status::Done) {
        std::cerr << "Critical Error: Could not bind to port " << port << std::endl;
        return -1;
    }
    std::cout << "Server started on port " << port << ". Waiting for players...\n";

    // Use std::list to prevent memory reallocation crashes
    std::list<sf::TcpSocket> clients;
    sf::SocketSelector selector;

    // Add the listener to the selector
    selector.add(listener);

    bool running = true;
    while (running) {
        // Wait for activity. Timeout after 10ms to keep the loop responsive
        if (selector.wait(sf::milliseconds(10))) {

            // CASE 1: A NEW PLAYER IS CONNECTING
            if (selector.isReady(listener)) {
                // Create a temporary placeholder in our list
                auto& newClient = clients.emplace_back();
                
                if (listener.accept(newClient) == sf::Socket::Status::Done) {
                    selector.add(newClient);
                    std::cout << "Player joined! Total players: " << clients.size() << "\n";
                    broadcast("A new player has joined the game.", clients);
                } else {
                    // If accept failed, clean up the empty list entry
                    clients.pop_back();
                }
            }
            
            // CASE 2: AN EXISTING PLAYER SENT DATA OR DISCONNECTED
            else {
                for (auto it = clients.begin(); it != clients.end(); ) {
                    sf::TcpSocket& client = *it;

                    if (selector.isReady(client)) {
                        sf::Packet packet;
                        sf::Socket::Status status = client.receive(packet);

                        if (status == sf::Socket::Status::Done) {
                            std::string receivedData;
                            if (packet >> receivedData) {
                                std::cout << "Relaying: " << receivedData << "\n";
                                broadcast(receivedData, clients);
                            }
                            ++it; // Move to next client
                        } 
                        else if (status == sf::Socket::Status::Disconnected || status == sf::Socket::Status::Error) {
                            // Handle cleanup to prevent crashing when accessing dead sockets
                            std::cout << "Player disconnected.\n";
                            selector.remove(client);
                            it = clients.erase(it); // Remove from list and update iterator
                            broadcast("A player has left the game.", clients);
                        } 
                        else {
                            ++it;
                        }
                    } else {
                        ++it;
                    }
                }
            }
        }
    }

    return 0;
}
/*
Memory Consistency: 
We use std::list. If you used std::vector, calling push_back could trigger a memory "reallocation," 
moving all sockets to new addresses. Since the selector holds pointers to the old addresses, 
the next wait() call would likely crash your program.
Iterator Safety: 
When a player disconnects, we use it = clients.erase(it). 
This is the standard C++ way to remove an item from a list while you are currently looping through it.
The Timeout: 
By passing sf::milliseconds(10) to selector.wait(), 
we ensure the server doesn't hang forever if no one is talking. 
This is crucial if you later want to add server-side logic (like moving NPCs).

*/