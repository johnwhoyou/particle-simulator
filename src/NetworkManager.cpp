#include "NetworkManager.h"

std::thread listeningThread;
std::thread heartbeatThread;
std::atomic<bool> listening{false};
std::atomic<bool> isRunning{false};

const int PACKET_SIZE = 16384;
const int CLIENT_PORT = 9000;

NetworkManager::NetworkManager(const std::string& serverIP, Uint16 serverTCPPort, Uint16 serverUDPPort) : tcpSocket(nullptr), udpSocket(nullptr) {
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

    if (SDLNet_ResolveHost(&serverUDPIP, serverIP.c_str(), serverUDPPort) == -1) {
        std::cerr << "SDLNet_ResolveHost UDP: " << SDLNet_GetError() << std::endl;
        SDLNet_UDP_Close(udpSocket);
        SDLNet_Quit();
        exit(-1);
    }

    if (SDLNet_ResolveHost(&serverTCPIP, serverIP.c_str(), serverTCPPort) == -1) {
        std::cerr << "SDLNet_ResolveHost TCP: " << SDLNet_GetError() << std::endl;
        SDLNet_Quit();
        exit(-1);
    }

    tcpSocket = SDLNet_TCP_Open(&serverTCPIP);
    if (!tcpSocket) {
        std::cerr << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
        SDLNet_Quit();
        exit(-1);
    }
}

NetworkManager::~NetworkManager() {
    stop();
    if (listeningThread.joinable()) {
        listeningThread.join();
    }
    if (heartbeatThread.joinable()) {
        heartbeatThread.join();
    }
    SDLNet_TCP_Close(tcpSocket);
    SDLNet_UDP_Close(udpSocket);
    SDLNet_Quit();
}

void NetworkManager::start() {
    isRunning = true;
    listening = true;
    listeningThread = std::thread(&NetworkManager::listen, this);
    heartbeatThread = std::thread(&NetworkManager::heartbeat, this);
}

void NetworkManager::stop() {
    isRunning = false;
    listening = false;
}

void NetworkManager::heartbeat() {
    const char* HEARTBEAT_MESSAGE = "HEARTBEAT\n";

    while (isRunning) {
        char response[1024];
        int receivedLength = SDLNet_TCP_Recv(tcpSocket, response, 1024);
        if (receivedLength > 0) {
            std::string receivedMsg(response, receivedLength);
            std::cout << receivedMsg << std::endl;
            if (receivedMsg.find(HEARTBEAT_MESSAGE) == std::string::npos) {
                std::cerr << "Heartbeat failed, server response invalid" << std::endl;
                break;
            }
            else {
                SDLNet_TCP_Send(tcpSocket, HEARTBEAT_MESSAGE, strlen(HEARTBEAT_MESSAGE));
            }
        } else {
            std::cerr << "Heartbeat failed, no response from server" << std::endl;
            break;
        }
    }
}

void NetworkManager::sendCommand(const char* command) {
    UDPpacket* packet = SDLNet_AllocPacket(PACKET_SIZE);
    if (!packet) {
        std::cerr << "SDLNet_AllocPacket: " << SDLNet_GetError() << std::endl;
        return;
    }

    memcpy(packet->data, command, strlen(command));
    packet->address = serverUDPIP;
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
            processReceivedData(receivedMessage);
        }

        SDLNet_FreePacket(packet);
    }
}

void NetworkManager::processReceivedData(const std::string& receivedData) {
    rapidjson::Document document;
    document.Parse(receivedData.c_str());

    if (document.HasParseError()) {
        std::cerr << "Error parsing received JSON data" << std::endl;
        return;
    }

    if (document.HasMember("particles") && document["particles"].IsArray()) {
        std::vector<std::pair<double, double>> serverParticles;

        const rapidjson::Value& particlesArray = document["particles"];
        for (rapidjson::SizeType i = 0; i < particlesArray.Size(); ++i) {
            const rapidjson::Value& particleObject = particlesArray[i];
            if (particleObject.IsObject()) {
                double x = particleObject["x"].GetDouble();
                double y = particleObject["y"].GetDouble();
                std::pair<double, double> newPair = std::make_pair(x, y);
                serverParticles.emplace_back(newPair);
            }
        }
        simulation->setServerParticles(serverParticles);
    }

    if (document.HasMember("sprites") && document["sprites"].IsArray()) {
        std::vector<Sprite> serverSprites;

        const rapidjson::Value& spritesArray = document["sprites"];
        for (rapidjson::SizeType i = 0; i < spritesArray.Size(); ++i) {
            const rapidjson::Value& spriteObject = spritesArray[i];
            if (spriteObject.IsObject()) {
                double x = spriteObject["x"].GetDouble();
                double y = spriteObject["y"].GetDouble();
                Sprite newSprite(x, y);
                serverSprites.emplace_back(newSprite);
            }
        }
        simulation->setServerSprites(serverSprites);
    }
}