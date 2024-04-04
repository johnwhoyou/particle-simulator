#include "NetworkManager.h"

NetworkManager::NetworkManager(const std::string& serverIP, Uint16 serverPort) : isRunning(false) {
    if (SDLNet_Init() == -1) {
        std::cerr << "SDLNet_Init: " << SDLNet_GetError() << std::endl;
        exit(-1);
    }

    udpSocket = SDLNet_UDP_Open(CLIENT_PORT);
    if (!udpSocket) {
        std::cerr << "SDLNet_UDP_Open: " << SDLNet_GetError() << std::endl;
        SDLNet_Quit();
        exit(-1);
    }

    if (SDLNet_ResolveHost(&this->serverIP, serverIP.c_str(), serverPort) == -1) {
        std::cerr << "SDLNet_ResolveHost: " << SDLNet_GetError() << std::endl;
        SDLNet_UDP_Close(udpSocket);
        SDLNet_Quit();
        exit(-1);
    }
}

NetworkManager::~NetworkManager() {
    stop();
    SDLNet_UDP_Close(udpSocket);
    SDLNet_Quit();
}

void NetworkManager::start() {
    isRunning = true;
    senderThread = std::thread(&NetworkManager::sendPackets, this);
    receiverThread = std::thread(&NetworkManager::receivePackets, this);
}

void NetworkManager::stop() {
    if (isRunning) {
        isRunning = false;
        senderThread.join();
        receiverThread.join();
    }
}

void NetworkManager::sendPackets() {
    UDPpacket* packet = SDLNet_AllocPacket(PACKET_SIZE);
    if (!packet) {
        std::cerr << "SDLNet_AllocPacket: " << SDLNet_GetError() << std::endl;
        return;
    }

    while (isRunning) {
        // Replace with actual data to send
        const char* message = "Hello from C++ client";
        memcpy(packet->data, message, strlen(message) + 1);
        packet->address = serverIP;
        packet->len = strlen(message) + 1;

        SDLNet_UDP_Send(udpSocket, -1, packet);
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Example sending rate
    }

    SDLNet_FreePacket(packet);
}

void NetworkManager::receivePackets() {
    UDPpacket* packet = SDLNet_AllocPacket(PACKET_SIZE);
    if (!packet) {
        std::cerr << "SDLNet_AllocPacket: " << SDLNet_GetError() << std::endl;
        return;
    }

    while (isRunning) {
        if (SDLNet_UDP_Recv(udpSocket, packet)) {
            std::cout << "Received: " << packet->data << std::endl;
        }
    }

    SDLNet_FreePacket(packet);
}
