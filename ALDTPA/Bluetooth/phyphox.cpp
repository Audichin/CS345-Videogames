#include <iostream>
#include <string>
#include <curl/curl.h>

// Callback to store incoming data into a std::string
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main() {
    // Replace this with your phone's IP + experiment endpoint
    std::string url = "http://192.168.1.64";

    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        std::cout << "Requesting data from: " << url << std::endl;

        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: "
                      << curl_easy_strerror(res) << std::endl;
        } else {
            std::cout << "Received JSON:\n" << readBuffer << std::endl;
        }

        curl_easy_cleanup(curl);
    }

    return 0;
}
