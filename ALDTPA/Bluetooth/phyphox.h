#pragma once

#include <string>
#include <iostream>
#include <thread>
#include <chrono>

#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
class Phyphox{
public:
    Phyphox(std::string ip) : BaseURL("http://" + ip + "/get"), prevAcc(0.0), prevGyro(0.0), pause(0)
    {
        curl = curl_easy_init();
    }

    ~Phyphox()
    {
        std::cout << "Deconstructor called, cleaning curl..." << std::endl;
        if (curl){
            curl_easy_cleanup(curl);
        }
    }

    int Phyphox_loop(int wait)
    {
        do
        {
            std::string recieve;
            std::string url = makeURL();
            
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // confused why things are broke
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &recieve);
            
            CURLcode res = curl_easy_perform(curl);
            if (res == CURLE_OK){
                JSON(recieve);
            }
            else 
            {
                pause = 0;
                if (res != CURLE_OK) 
                {
                    wait++;
                    std::cout << "Currl error, waiting: " << 5 - wait << std::endl;
                }
                if (wait == 5)
                {
                    std::cout << "[ERR]: Lost connection to phone, please check connection and reset phone graphs..." << std::endl;
                    return -1; // temp for now, hope to reset makeURL to base state and allow game to continue after fixing connection
                }
            }
        std::this_thread::sleep_for(std::chrono::milliseconds(wait));
        }while(true);
    }

private:
    CURL *curl;
    std::string BaseURL;
    float prevAcc;
    float prevGyro;

    int pause;

    std::string makeURL()
    {
        return BaseURL + "?accX=" + std::to_string(prevAcc) + "|acc_time" +
                         "&accY=" + std::to_string(prevAcc) + "|acc_time" +
                         "&accZ=" + std::to_string(prevAcc) + "|acc_time" +
                         "&acc_time=" + std::to_string(prevAcc) +
                         "&gyroX=" + std::to_string(prevGyro) + "|gyro_time" +
                         "&gyroY=" + std::to_string(prevGyro) + "|gyro_time" +
                         "&gyroZ=" + std::to_string(prevGyro) + "|gyro_time" +
                         "&gyro_time=" + std::to_string(prevGyro);
    }

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
        size_t totalSize = size * nmemb;
        std::string* str = static_cast<std::string*>(userp);
        str->append(static_cast<char*>(contents), totalSize);
        return totalSize;
    }
    void JSON(std::string response)
    {
        json j = json::parse(response);
        try{
            std::cout << j << std::endl;
        }
        catch(...){
            std::cout << "[WARN]: JSON could not parse most recent Phyphox data..." << std::endl;
        }
        
    }
};