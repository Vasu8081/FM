#ifndef ONEDRIVE_H
#define ONEDRIVE_H

#include <iostream>
#include <string>
#include <curl/curl.h>
#include <json.hpp>
#include <app_config.hpp>

class onedrive
{
private:
    onedrive()
    {
        if (_config.getRefreshToken().empty())
        {
            std::string deviceCode, userCode;
            int interval;
            std::string deviceCodeStr = getDeviceCode(userCode, interval);
            if (deviceCodeStr.empty())
            {
                std::cerr << "Error: Failed to get device code" << std::endl;
                return;
            }

            auto accessToken = getAccessToken(deviceCodeStr, interval);
            if (accessToken.empty())
            {
                std::cerr << "Error: Failed to get access token" << std::endl;
                return;
            }
        }
        else
        {
            auto accessToken = refreshAccessToken();
            if (accessToken.empty())
            {
                std::cerr << "Error: Failed to refresh access token" << std::endl;
                return;
            }
        }
        sync_down();
    }

    ~onedrive()
    {
        sync_up();
    }

    app_config &_config = app_config::getInstance();

    // Helper functions
    static size_t writeCallback(void *contents, size_t size, size_t nmemb, std::string *userData);
    static size_t writeFileCallback(void *ptr, size_t size, size_t nmemb, FILE *stream);

    std::string getDeviceCode(std::string &userCode, int &interval);
    std::string getAccessToken(const std::string &deviceCode, int interval);
    std::string refreshAccessToken();
    void uploadFile(const std::string &localPath, const std::string &oneDrivePath);
    void downloadFile(const std::string &oneDrivePath, const std::string &localPath);

public:
    onedrive(const onedrive &) = delete;
    onedrive &operator=(const onedrive &) = delete;

    static onedrive &getInstance()
    {
        static onedrive instance;
        return instance;
    }

    void sync_up();
    void sync_down();
};

#endif // ONEDRIVE_H
