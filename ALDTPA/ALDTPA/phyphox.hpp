#pragma once

#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>

#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
class Phyphox
{
public:
    struct IMUData
    {
        float direct;
        float yaw;
        float pitch;
        float roll;
        float attT;
        bool measuring;
        bool warn;
        int err;
        int wait;
    };

    Phyphox(std::string ip)
        : BaseURL("http://" + ip + "/get"),
          prevDirect(0.0),
          prevYaw(0.0),
          prevPitch(0.0),
          prevRoll(0.0),
          prevAttT(0.0),
          pause(0),
          wait(0)
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

    std::string Get_BaseURL()
    {
        return BaseURL;
    }

    float Get_prevDirect()
    {
        return prevDirect;
    }

    float Get_prevYaw()
    {
        return prevYaw;
    }

    float Get_prevPitch()
    {
        return prevPitch;
    }

    float Get_prevRoll()
    {
        return prevRoll;
    }

    float Get_wait()
    {
        return wait;
    }

    float Get_pause()
    {
        return pause;
    }

    void Set_BaseURL(std::string baseurl)
    {
        BaseURL = baseurl;
    }

    void Set_prevDirect(float prevdirect)
    {
        prevDirect = prevdirect;
    }

    void Set_prevYaw(float prevyaw)
    {
        prevYaw = prevyaw;
    }

    void Set_prevPitch(float prevpitch)
    {
        prevPitch = prevpitch;
    }

    void Set_prevRoll(float prevroll)
    {
        prevRoll = prevroll;
    }

    IMUData Phyphox_loop()
    {
        std::string recieve;
        IMUData data{};

        std::string url = makeURL();

        curl_easy_reset(curl);

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &recieve);

        CURLcode res = curl_easy_perform(curl);

        if (res == CURLE_OK)
        {
            wait = 0;
            data.wait = 0;
            data = JSON(recieve);

            prevDirect = data.direct;
            prevYaw = data.yaw;
            prevPitch = data.pitch;
            prevRoll = data.roll;
            prevAttT = data.attT;
        }
        else
        {
            wait += 1;
            data.wait = wait;

            if (wait >= 3)
            {
                std::cout << "[WARN 1]: Curl cannot find connection (3/3) " << std::endl;
                std::cout << "[ERR]: Lost connection to phone, closing game..." << std::endl;
                data.err = 1;
                return data;
            }

            std::cout << "[WARN 1]: Curl cannot find connection (" << wait << "/3)" << std::endl;
            return data;
        }
        return data;
    }

    void ClearBuffers()
    {
        std::string url = BaseURL.substr(0, BaseURL.find("/get")) + "/control?cmd=clear";

        curl_easy_reset(curl);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        curl_easy_perform(curl);
    }

private:
    CURL *curl;
    std::string BaseURL;
    float prevDirect;
    float prevYaw;
    float prevPitch;
    float prevRoll;
    float prevAttT;

    int pause;
    int wait = 0;

    float rounded(float value)
    {
        return (std::round(value * 10000.0) / 10000.0);
    }

    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
    {
        size_t totalSize = size * nmemb;
        std::string *str = static_cast<std::string *>(userp);
        str->append(static_cast<char *>(contents), totalSize);
        return totalSize;
    }

    std::string makeURL()
    {
        return BaseURL + "?direct=" + std::to_string(prevDirect) + "|attT" +
               "&attT=" + std::to_string(prevAttT) +
               "&yaw=" + std::to_string(prevYaw) + "|attT" +
               "&pitch=" + std::to_string(prevPitch) + "|attT" +
               "&roll=" + std::to_string(prevRoll) + "|attT";
    }

    IMUData JSON(std::string response)
    {
        json j = json::parse(response);
        auto &buffer = j["buffer"];
        bool measuring;
        IMUData data{};

        try
        {
            if (j.contains("status"))
            {
                measuring = j["status"]["measuring"];
                data.measuring = j["status"]["measuring"];
                if (measuring == true)
                {
                    if (buffer.contains("direct"))
                    {
                        auto direct = buffer["direct"]["buffer"];
                        if (!direct.empty())
                        {
                            data.direct = rounded(static_cast<float>(direct.back()));
                        }
                        // std::cout << "Direct  | "
                        //           << data.direct << std::endl;
                    }

                    if (buffer.contains("attT"))
                    {
                        auto attT = buffer["attT"]["buffer"];
                        if (!attT.empty())
                        {
                            data.attT = rounded(static_cast<float>(attT.back()));
                        }
                        // std::cout << "Timestamp | "
                        //           << data.yaw << ", "
                        //           << data.pitch << ", "
                        //           << data.roll << std::endl;
                    }
                    if (buffer.contains("pitch"))
                    {
                        auto pitch = buffer["pitch"]["buffer"];
                        if (!pitch.empty())
                        {
                            data.pitch = rounded(static_cast<float>(pitch.back()));
                        }
                        // std::cout << "Pitch  | "
                        //           << data.pitch << std::endl;
                    }
                    if (buffer.contains("roll"))
                    {
                        auto roll = buffer["roll"]["buffer"];
                        if (!roll.empty())
                        {
                            data.roll = rounded(static_cast<float>(roll.back()));
                        }
                        // std::cout << "Roll  | "
                        //           << data.roll << std::endl;
                    }
                    if (buffer.contains("yaw"))
                    {
                        auto yaw = buffer["yaw"]["buffer"];
                        if (!yaw.empty())
                        {
                            data.yaw = rounded(static_cast<float>(yaw.back()));
                        }
                            // std::cout << "Yaw  | "
                            //         << data.yaw << std::endl;
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
        catch (int err)
        {
            if (err == 1)
            {
                std::cout << "[WARN 2]: JSON could not parse most recent Phyphox data..." << std::endl;
                data.warn = true;
            }
            if (err == 2)
            {
                std::cout << "[WARN 3]: No data from Phyphox, unpause Phyphox on phone..." << std::endl;
                data.warn = true;
            }
        }
        return data;
    }
};
