/*
To move beyond strings, you must define a data protocol. By overloading the << and >> operators, you teach sf::Packet exactly how to "shred" your struct into bytes and how to "rebuild" it on the other side.
1. The Shared Data Structure
Create a shared header (e.g., Protocol.hpp) that both your server and client use. Using fixed-width types like sf::Uint32 is critical to prevent crashes between 32-bit and 64-bit systems [1].

*/
#include <SFML/Network.hpp>

struct PlayerUpdate {
    sf::Uint32 id;
    float x, y;
    sf::Uint32 health;

    // Overload the << operator to pack data
    friend sf::Packet& operator <<(sf::Packet& packet, const PlayerUpdate& data) {
        return packet << data.id << data.x << data.y << data.health;
    }

    // Overload the >> operator to unpack data
    friend sf::Packet& operator >>(sf::Packet& packet, PlayerUpdate& data) {
        return packet >> data.id >> data.x >> data.y >> data.health;
    }
};

/*
2. The Server Logic (Processing the Struct)
The server receives the packet and extracts the struct directly. It then broadcasts the updated position to all other players.

*/

// Inside the server's receive loop...
sf::Packet packet;
if (client.receive(packet) == sf::Socket::Status::Done) {
    PlayerUpdate update;
    
    // The >> operator now knows how to handle the whole struct!
    if (packet >> update) {
        std::cout << "Player " << update.id << " moved to " << update.x << "," << update.y << "\n";
        
        // Broadcast the update to everyone else
        sf::Packet relayPacket;
        relayPacket << update; 
        for (auto& otherClient : clients) {
            otherClient.send(relayPacket);
        }
    }
}

/*
3. The Client Logic (Sending and Receiving)
The client sends its own position and updates its local representation of "other players" based on what the server sends back.
*/

// --- Inside the Client Game Loop ---

// 1. Send our data to server
PlayerUpdate myStatus = { 1, currentX, currentY, 100 };
sf::Packet sendPacket;
sendPacket << myStatus;
socket.send(sendPacket);

// 2. Receive other players' data (Non-blocking)
sf::Packet recvPacket;
if (socket.receive(recvPacket) == sf::Socket::Status::Done) {
    PlayerUpdate incomingUpdate;
    if (recvPacket >> incomingUpdate) {
        // Update the visual position of the 'incomingUpdate.id' player
        remotePlayers[incomingUpdate.id].setPosition({incomingUpdate.x, incomingUpdate.y});
    }
}

/*
Key Safety Tips for Complex Data
Version Matching: If you change the struct (e.g., add armor), you must recompile both the client and server. If their versions don't match, the >> operator will read the wrong number of bytes and cause a logic crash [2].
Packet Type ID: In a real game, you’ll send many types of data (Chat, Movement, Disconnect). Use an enum as the first item in every packet so the receiver knows which struct to extract [3].
*/
