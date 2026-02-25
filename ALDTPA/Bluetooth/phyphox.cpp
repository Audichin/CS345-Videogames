#include "./Phyphox.h"
#include <iostream>

int main() {
    std::string ip;
    std::cout << "[NOTICE]: Pleas make sure you are connected to the same wifi for your phone and PC" << std::endl;
    std::cout << "Enter Phyphox IP (example 192.168.0.5:8080): ";
    std::cin >> ip;

    Phyphox poller(ip);

    std::cout << "Starting poll...\n";
    poller.Phyphox_loop(50);
    
    return 0;
} 