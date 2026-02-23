#include "Phyphox.h"
#include <iostream>

int main() {
    std::string ip;
    std::cout << "Enter Phyphox IP (example 192.168.0.5:8080): ";
    std::cin >> ip;

    PhyphoxPoller poller(ip);

    std::cout << "Starting poll...\n";
    poller.pollLoop(50);

    return 0;
} 