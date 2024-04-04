#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "SDL_net.h"
#include "json.hpp"
#include <string>
#include <thread>
#include <iostream>
#include <atomic>
#include <chrono>

class NetworkManager {
public:
    NetworkManager(const std::string& serverIP, Uint16 serverTCPPort, Uint16 serverUDPPort);
    ~NetworkManager();

    void start();
    void stop();

    void sendCommand(const char* command);

private:
    IPaddress serverUDPIP;
    IPaddress serverTCPIP;
    TCPsocket tcpSocket;
    UDPsocket udpSocket;
    std::atomic<bool> isRunning;
    std::atomic<bool> listening;

    void heartbeat();
    void listen();
};

#endif // NETWORKMANAGER_H
