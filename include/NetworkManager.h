#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "SDL_net.h"
#include "json.hpp"
#include <string>
#include <thread>
#include <iostream>
#include <atomic>

class NetworkManager {
public:
    NetworkManager(const std::string& serverIP, Uint16 serverPort);
    ~NetworkManager();

    void start();
    void stop();

    void sendCommand(const char* command);

private:
    const int PACKET_SIZE = 512;
    IPaddress serverIP;
    UDPsocket udpSocket;
    std::thread senderThread;
    std::thread receiverThread;
    bool isRunning;
    void listen();
};

#endif // NETWORKMANAGER_H
