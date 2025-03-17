#ifndef UPSTOX_H
#define UPSTOX_H

#include <string>
#include <curl/curl.h>
#include <wx/wx.h>
#include <app_config.hpp>
#include <cpr/cpr.h>
#include <crow.h>
#include <thread>
#include <atomic>
#include <chrono>
#include <future>

using nlohmann::json;

class Upstox
{
public:
    Upstox(const Upstox &) = delete;
    Upstox &operator=(const Upstox &) = delete;

    static Upstox &getInstance()
    {
        static Upstox instance;
        return instance;
    }

    bool isActive() { return _active; }

    json getProduct(const std::string &symbol)
    {
        std::string url = "https://api-v2.upstox.com/market-quote/quotes?symbol=" + symbol;
        std::string response;
        CURL *curl = curl_easy_init();

        if (curl)
        {
            struct curl_slist *headers = nullptr;
            headers = curl_slist_append(headers, "Api-Version: 2.0");
            headers = curl_slist_append(headers, "Accept: application/json");
            headers = curl_slist_append(headers, ("Authorization: Bearer " + _access_token).c_str());

            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            CURLcode res = curl_easy_perform(curl);

            if (res != CURLE_OK)
            {
                wxMessageBox("CURL request failed: %s" + std::string(curl_easy_strerror(res)));
                wxLogError("CURL request failed: %s", curl_easy_strerror(res));
            }

            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
        }

        try
        {
            return json::parse(response);
        }
        catch (const json::exception &e)
        {
            wxMessageBox("JSON parsing error: %s" + std::string(e.what()));
            return {};
        }
    }

private:
    app_config &_config = app_config::getInstance();

    std::string _access_token;
    std::string _base_url = "https://api-v2.upstox.com"; // Replace with actual API base URL
    std::string _auth_code;
    bool _active = false;

    bool checkAccessToken() {
        cpr::Response response = cpr::Get(cpr::Url{_base_url + "/portfolio/long-term-holdings"}, cpr::Header{{"Authorization", "Bearer " + _access_token}});
        return response.status_code == 401;
    }

    std::atomic<bool> auth_received{false};  // Shared flag to stop the server

    void startLocalServer() {
        crow::SimpleApp app;
    
        CROW_ROUTE(app, "/callback")
        ([&](const crow::request& req) {
            auto query_params = crow::query_string(req.url_params);
            if (query_params.get("code")) {
                _auth_code = query_params.get("code");
                auth_received.store(true);
                return "Authorization successful! You can close this window.";
            }
            return "Failed to retrieve authorization code.";
        });
    
        std::thread server_thread([&]() {
            app.port(8080).multithreaded().run();
        });
    
        while (!auth_received.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    
        app.stop();
        server_thread.join();
    }

    std::string getAuthorizationCode() {
        std::string auth_url = _base_url + "/login/authorization/dialog?client_id=a373377f-1b34-40f7-923c-0f23f0fec7dd&redirect_uri=http://localhost:8080/callback";
        std::string command;
        #ifdef _WIN32
            command = "start \"\" \"" + auth_url + "\"";  // Windows needs extra quoting
        #elif __APPLE__
            command = "open \"" + auth_url + "\"";  // macOS
        #else
            command = "xdg-open \"" + auth_url + "\"";  // Linux
        #endif
    
        system(command.c_str());
        startLocalServer();
        return _auth_code;
    }
    
    std::string requestNewAccessToken(const std::string& code) {
        cpr::Response response = cpr::Post(
            cpr::Url{_base_url + "/login/authorization/token"},
            cpr::Payload{
                {"client_id", "a373377f-1b34-40f7-923c-0f23f0fec7dd"},
                {"client_secret", "hwm3xojo9h"},
                {"code", code},
                {"redirect_uri", "http://localhost:8080/callback"},
                {"grant_type", "authorization_code"}
            },
            cpr::Header{{"Content-Type", "application/x-www-form-urlencoded"}} 
        );
    
        if (response.status_code == 200) {
            try {
                json jsonResponse = json::parse(response.text);
                if (jsonResponse.contains("access_token")) {
                    return jsonResponse["access_token"].get<std::string>();
                } else {
                    std::cerr << "Error: No access_token found in response." << std::endl;
                }
            } catch (json::parse_error& e) {
                std::cerr << "JSON Parse Error: " << e.what() << std::endl;
            }
        } else {
            std::cerr << "Failed to get access token! HTTP Code: " << response.status_code << std::endl;
            std::cerr << "Response: " << response.text << std::endl;
        }
    
        return "";
    }
    

    Upstox() {
        _access_token = _config.getUpstoxAccessToken();

        if (_access_token.empty() || checkAccessToken()) {

            std::string _auth_code = getAuthorizationCode();
            if (!_auth_code.empty()) {
                std::string new_token = requestNewAccessToken(_auth_code);
                if (!new_token.empty()) {
                    _config.setUpstoxAccessToken(new_token);
                    _config.save();
                    _access_token = new_token;
                    _active = true;
                }
            }
        }
        else{
            _active = true;
        }

    }

    static size_t writeCallback(void *contents, size_t size, size_t nmemb, std::string *output)
    {
        size_t totalSize = size * nmemb;
        output->append((char *)contents, totalSize);
        return totalSize;
    }
};

#endif
