#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <string>
#include <iostream>

#include "phyphox.h"

using json = nlohmann::json;

int main() {
    Phyphox px;
    px.curl = curl_easy_init();
    std::cout << "[NOTICE]: Make sure your phone and PC are using the same Wi-fi" << std::endl;
    std::cout << "Paste Phyphox address numbers:";
    std::cin >> px.URL_temp;
    px.URL = "http://" + px.URL_temp + "/get?buffer=accX&buffer=accY&buffer=accZ&buffer=gyrX&buffer=gyrY&buffer=gyrZ";

    if(px.curl) {
        curl_easy_setopt(px.curl, CURLOPT_URL, px.URL.c_str());
        curl_easy_setopt(px.curl, CURLOPT_WRITEFUNCTION, px.WriteCallback);
        curl_easy_setopt(px.curl, CURLOPT_WRITEDATA, &px.readBuffer);

        px.res = curl_easy_perform(px.curl);
        curl_easy_cleanup(px.curl);
    }
    else
    {
        std::cout << "[ERR]: curl failed to initalize upon runtime" << std::endl;
    }

    std::cout << px.readBuffer << std::endl;

    return 0;
}
