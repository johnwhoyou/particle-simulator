#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "SDL_net.h"
#include <string>
#include <thread>
#include <iostream>

class NetworkManager {
public:
    NetworkManager(const std::string& serverIP, Uint16 serverPort);
    ~NetworkManager();

    void start();
    void stop();

private:
    const int CLIENT_PORT = 1234;
    const int PACKET_SIZE = 512;
    IPaddress serverIP;
    UDPsocket udpSocket;
    std::thread senderThread;
    std::thread receiverThread;
    bool isRunning;

    void sendPackets();
    void receivePackets();
};

#endif // NETWORKMANAGER_H
