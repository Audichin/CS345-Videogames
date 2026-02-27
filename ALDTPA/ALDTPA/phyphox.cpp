#include <curl/curl.h>
#include "phyphox.h"

using namespace std; 

int phy_main() 
{
std::string ip;
std::cout << "[NOTICE]: Please make sure you are connected to the same wifi for your phone and PC" << std::endl;
std::cout << "Enter Phyphox IP (Ex 192.168.1.152): ";
std::cin >> ip;

Phyphox poller(ip);

std::cout << "Starting poll...\n";
poller.Phyphox_loop(50);

return 0;
}   