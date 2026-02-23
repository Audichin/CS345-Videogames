#pragma once
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

using json = nlohmann::json;

class PhyphoxPoller {
public:
    PhyphoxPoller(const std::string& ip)
        : baseURL("http://" + ip + "/get"),
          lastAccTime(0.0),
          lastGyroTime(0.0)
    {
        curl = curl_easy_init();
    }

    ~PhyphoxPoller() {
        if (curl)
            curl_easy_cleanup(curl);
    }

    void pollLoop(int intervalMs = 50) {
        while (true) {
            std::string response;
            std::string url = buildURL();

            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

            CURLcode res = curl_easy_perform(curl);
            if (res == CURLE_OK) {
                parseAndPrint(response);
            } else {
                std::cerr << "Curl error\n";
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
        }
    } 

private:
    CURL* curl;
    std::string baseURL;
    double lastAccTime;
    double lastGyroTime;

    static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
        size_t totalSize = size * nmemb;
        std::string* str = static_cast<std::string*>(userp);
        str->append(static_cast<char*>(contents), totalSize);
        return totalSize;
    }

    std::string buildURL() {
        return baseURL +
            "?accX=" + std::to_string(lastAccTime) + "|acc_time" +
            "&accY=" + std::to_string(lastAccTime) + "|acc_time" +
            "&accZ=" + std::to_string(lastAccTime) + "|acc_time" +
            "&acc_time=" + std::to_string(lastAccTime) +
            "&gyroX=" + std::to_string(lastGyroTime) + "|gyro_time" +
            "&gyroY=" + std::to_string(lastGyroTime) + "|gyro_time" +
            "&gyroZ=" + std::to_string(lastGyroTime) + "|gyro_time" +
            "&gyro_time=" + std::to_string(lastGyroTime);
    }

    void parseAndPrint(const std::string& response) {
        try {
            json j = json::parse(response);

            auto& buffer = j["buffer"];

            if (buffer.contains("acc_time")) {
                auto accTimes = buffer["acc_time"]["buffer"];
                auto accX = buffer["accX"]["buffer"];
                auto accY = buffer["accY"]["buffer"];
                auto accZ = buffer["accZ"]["buffer"];

                if (!accTimes.empty()) {
                    lastAccTime = accTimes.back();

                    std::cout << "ACC  | "
                              << accX.back() << ", "
                              << accY.back() << ", "
                              << accZ.back() << std::endl;
                }
            }

            if (buffer.contains("gyro_time")) {
                auto gyroTimes = buffer["gyro_time"]["buffer"];
                auto gyroX = buffer["gyroX"]["buffer"];
                auto gyroY = buffer["gyroY"]["buffer"];
                auto gyroZ = buffer["gyroZ"]["buffer"];

                if (!gyroTimes.empty()) {
                    lastGyroTime = gyroTimes.back();

                    std::cout << "GYRO | "
                              << gyroX.back() << ", "
                              << gyroY.back() << ", "
                              << gyroZ.back() << std::endl;
                }
            }
        }
        catch (...) {
            std::cerr << "JSON parse error\n";
        }
    }
};