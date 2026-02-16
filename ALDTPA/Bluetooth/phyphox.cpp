#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <string>
#include <iostream>

using json = nlohmann::json;

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

int main() {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;
    
    std::cout << "Testing!" << std::endl;
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://10.4.46.233/get?buffer=accX&buffer=accY&buffer=accZ&buffer=gyrX&buffer=gyrY&buffer=gyrZ");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    std::cout << readBuffer << std::endl;
}
