#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

using json = nlohmann::json;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realSize = size * nmemb;
    std::string* s = static_cast<std::string*>(userp);
    s->append(static_cast<char*>(contents), realSize);
    return realSize;
}

int main() {
    std::string host;
    std::cout << "[NOTICE] Make sure phone & PC are on same Wi-Fi\n";
    std::cout << "Enter Phyphox address (e.g. 192.168.1.152:8080 or 192.168.1.152): ";
    std::cin >> host;

    // build base URL (make sure port if needed)
    std::string base = "http://" + host;
    // buffers we want:
    std::vector<std::string> names = {"accX","accY","accZ","gyroX","gyroY","gyroZ"};

    // build buffer query part
    std::string buffers_q;
    for (size_t i = 0; i < names.size(); ++i) {
        if (i) buffers_q += "&";
        buffers_q += "buffer=" + names[i];
    }

    // we add clear=true to fetch-and-clear each poll
    std::string url = base + "/get?" + buffers_q + "&clear=true";

    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "curl init failed\n";
        return 1;
    }

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    // optional: set a reasonable timeout
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 2000L);

    std::string readBuffer;

    std::cout << "Polling " << url << "  (ctrl-C to stop)\n";

    while (true) {
        readBuffer.clear();
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl error: " << curl_easy_strerror(res) << "\n";

            continue;
        }

        // parse JSON (guarded)
        try {
            auto j = json::parse(readBuffer);

            if (!j.contains("buffer") || j["buffer"].empty()) {
                // nothing available right now
                // (that's expected sometimes)
            } else {
                for (auto &name : names) {
                    if (j["buffer"].contains(name) && j["buffer"][name].contains("buffer")) {
                        auto arr = j["buffer"][name]["buffer"];
                        if (!arr.empty()) {
                            // example: print all samples, or just the last one:
                            std::cout << name << " got " << arr.size() << " samples. latest = "
                                      << arr.back() << "\n";
                        }
                    }
                }
            }
        } catch (const std::exception &e) {
            std::cerr << "JSON parse error: " << e.what() << "\n";
            std::cerr << "Raw: " << readBuffer << "\n";
        }

        // poll rate (adjust as needed)
    }

    curl_easy_cleanup(curl);
    return 0;
}