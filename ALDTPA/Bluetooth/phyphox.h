#pragma once
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Phyphox
{
    private:

    public:
        CURL* curl;
        CURLcode res;
        std::string readBuffer;
        json j;
        std::string URL_temp;
        std::string URL;

        static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) 
        {
            size_t totalSize = size * nmemb;
            output->append((char*)contents, totalSize);
            return totalSize;
        }
    
    Phyphox()
    {

    }
};
