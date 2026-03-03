#pragma once

#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>

#include "curl/curl.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;
class Phyphox{
public:
    struct IMUData
    { 
        float ax, ay, az; 
        float gx, gy, gz; 
        bool measuring;
        int err;
    };

    Phyphox(std::string ip) : BaseURL("http://" + ip + "/get"), prevAcc(0.0), prevGyro(0.0), pause(0), wait(0)
    {
        curl = curl_easy_init();
    }

    ~Phyphox()
    {
        std::cout << "[NOTICE]: Deconstructor called, cleaning curl..." << std::endl;
        if (curl)
        {
            curl_easy_cleanup(curl);
        }
    }

    IMUData Phyphox_loop(int wait)
    {
        wait = 0;
        std::string recieve;
        Phyphox::IMUData data;
        std::string url = makeURL();
        
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // confused why things are broke
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &recieve);
        
        CURLcode res = curl_easy_perform(curl);
        std::cout << "TEST" << std::endl;
        if (res == CURLE_OK){

            data = JSON(recieve);
            wait = 0;
        }
        else 
        {
            // if (wait == 3)
            // {
            //     std::cout << "[ERR]: Lost connection to phone, please check connection and reset phone graphs..." << std::endl;
            //     data.err = -1;
            //     return data; // temp for now, hope to reset makeURL to base state and allow game to continue after fixing connection
            // }
            if (res != CURLE_OK) 
            {
                std::cout << "[WARN 1]: Curl cannot find connection" << std::endl;
                // wait++;
                Set_prevAcc(0.0);
                Set_prevGyro(0.0);

                std::this_thread::sleep_for(std::chrono::milliseconds(wait));
            } 
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(wait));
    return data;
    }

private:
    CURL *curl;
    std::string BaseURL;
    float prevAcc;
    float prevGyro;
    int pause;
    int wait = 0;


    std::string Get_BaseURL()
    {
        return BaseURL;
    }

    float Get_prevAcc()
    {
        return prevAcc;
    }

    float Get_prevGyro()
    {
        return prevGyro;
    }

    void Set_BaseURL(std::string baseurl)
    {
        BaseURL = baseurl;
    }

    void Set_prevAcc(float prevacc)
    {
        prevAcc = prevacc;
    }

    void Set_prevGyro(float prevgyro)
    {
        prevGyro = prevgyro;
    }

    float rounded(float value)
    {
        return(std::round(value * 10000.0) / 10000.0);
    }

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) 
    {
        size_t totalSize = size * nmemb;
        std::string* str = static_cast<std::string*>(userp);
        str->append(static_cast<char*>(contents), totalSize);
        return totalSize;
    }

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

    IMUData JSON(std::string response)
    {
        json j = json::parse(response);
        auto& buffer = j["buffer"];
        bool measuring;
        IMUData data;
        try
        {
            if (j.contains("status"))
            {
                measuring = j["status"]["measuring"];
                data.measuring = j["status"]["measuring"];
                if (measuring == true)
                {
                    if (buffer.contains("acc_time")) 
                    {
                        auto accTimes = buffer["acc_time"]["buffer"];
                        auto accX = buffer["accX"]["buffer"];
                        auto accY = buffer["accY"]["buffer"];
                        auto accZ = buffer["accZ"]["buffer"];

                        data.ax =  rounded(static_cast<float>(accX.back()));
                        data.ay =  rounded(static_cast<float>(accY.back()));
                        data.az =  rounded(static_cast<float>(accZ.back()));

                        std::cout << "ACC  | "
                              << data.ax << ", "
                              << data.ay << ", "
                              << data.az << std::endl;
                    }

                    if (buffer.contains("gyro_time")) 
                    {
                        auto gyroTimes = buffer["gyro_time"]["buffer"];
                        auto gyroX = buffer["gyroX"]["buffer"];
                        auto gyroY = buffer["gyroY"]["buffer"];
                        auto gyroZ = buffer["gyroZ"]["buffer"];

                        data.gx =  rounded(static_cast<float>(gyroX.back()));
                        data.gy =  rounded(static_cast<float>(gyroY.back()));
                        data.gz =  rounded(static_cast<float>(gyroZ.back()));

                        std::cout << "GYRO | "
                              << data.gx << ", "
                              << data.gy << ", "
                              << data.gz << std::endl; 
                    }
                }
                else
                {
                    throw 2; 
                }
            }
            else
            {
                throw 1;
            }
        }
        catch(int err)
        {
            if (err == 1)
            {
                std::cout << "[WARN 2]: JSON could not parse most recent Phyphox data..." << std::endl;
            }
            if (err == 2)
            {
                std::cout << "[WARN 3]: No data from Phyphox, unpause Phyphox on phone..." << std::endl;   
            }
        }
        return data;
    }
};  