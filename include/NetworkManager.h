#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "Simulation.h"
#include "SDL_net.h"
#include "Sprite.h"
#include <string>
#include <thread>
#include <iostream>
#include <atomic>
#include <chrono>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

class NetworkManager {
public:
    NetworkManager(const std::string& serverIP, Uint16 serverTCPPort, Uint16 serverUDPPort);
    ~NetworkManager();

    void start();
    void stop();

    void sendCommand(const char* command);

    void setSimulation(Simulation* sim) {
        simulation = sim;
    }

private:
    Simulation* simulation;

    IPaddress serverUDPIP;
    IPaddress serverTCPIP;
    TCPsocket tcpSocket;
    UDPsocket udpSocket;
    std::atomic<bool> isRunning;
    std::atomic<bool> listening;

    void heartbeat();
    void listen();
    void processReceivedData(const std::string& receivedData);

};

#endif // NETWORKMANAGER_H
