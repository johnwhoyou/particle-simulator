#include "NetworkManager.h"

std::thread listeningThread;
std::atomic<bool> listening{false};

NetworkManager::NetworkManager(const std::string& serverIP, Uint16 serverPort) : isRunning(false), udpSocket(nullptr) {
    if (SDLNet_Init() == -1) {
        std::cerr << "SDLNet_Init: " << SDLNet_GetError() << std::endl;
        exit(-1);
    }

    udpSocket = SDLNet_UDP_Open(9000); 
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
    if (listeningThread.joinable()) {
        listeningThread.join();
    }
    SDLNet_UDP_Close(udpSocket);
    SDLNet_Quit();
}

void NetworkManager::start() {
    isRunning = true;
    listening = true;
    listeningThread = std::thread(&NetworkManager::listen, this);
}

void NetworkManager::stop() {
    if (isRunning) {
        isRunning = false;
    }
    listening = false;
}

void NetworkManager::sendCommand(const char* command) {
    UDPpacket* packet = SDLNet_AllocPacket(PACKET_SIZE);
    if (!packet) {
        std::cerr << "SDLNet_AllocPacket: " << SDLNet_GetError() << std::endl;
        return;
    }

    memcpy(packet->data, command, strlen(command));
    packet->address = serverIP;
    packet->len = strlen(command);

    if (SDLNet_UDP_Send(udpSocket, -1, packet) == 0) {
        std::cerr << "SDLNet_UDP_Send: " << SDLNet_GetError() << std::endl;
    }

    SDLNet_FreePacket(packet);
}

void NetworkManager::listen() {
    while (listening) {
        UDPpacket* packet = SDLNet_AllocPacket(PACKET_SIZE);
        if (!packet) {
            std::cerr << "SDLNet_AllocPacket: " << SDLNet_GetError() << std::endl;
            continue;
        }

        if (SDLNet_UDP_Recv(udpSocket, packet)) {
            std::string receivedMessage(reinterpret_cast<char*>(packet->data), packet->len);
            std::cout << "Received message from server: " << receivedMessage << std::endl;
        }

        SDLNet_FreePacket(packet);
    }
}