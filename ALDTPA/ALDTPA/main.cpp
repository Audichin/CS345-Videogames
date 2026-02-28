#include <SDL.h>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iostream>

#include "phyphox.cpp"
#include "main.h"

using namespace std; 
 
 
int main(int argc, char* argv[])
{
    phy_main();
    return 0;
}
