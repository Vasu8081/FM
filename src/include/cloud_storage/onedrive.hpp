#ifndef ONEDRIVE_H
#define ONEDRIVE_H

#include <iostream>
#include <string>
#include <curl/curl.h>
#include <json.hpp>
#include <app_config.hpp>
#include <crow.h>

class onedrive
{
private:
    onedrive()
    {
        if (_config.getRefreshToken().empty())
        {
            auto authCode = waitForAuthCode();
            if (authCode.empty())
            {
                std::cerr << "Error: Failed to get authorization code" << std::endl;
                return;
            }

            auto accessToken = getAccessToken(authCode);
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
        syncFiles();
    }

    ~onedrive()
    {
        syncFiles();
    }

    app_config &_config = app_config::getInstance();

    // Helper functions
    static size_t writeCallback(void *contents, size_t size, size_t nmemb, std::string *userData);
    static size_t writeFileCallback(void *ptr, size_t size, size_t nmemb, FILE *stream);

    void startLocalServer();
    std::string waitForAuthCode();
    std::string getAccessToken(const std::string &authCode);
    std::string refreshAccessToken();
    bool uploadFile(const std::string &localPath, const std::string &oneDrivePath);
    bool downloadFile(const std::string &oneDrivePath, const std::string &localPath);
    void retryDownload(const std::string &serverPath, const std::string &localPath, int maxRetries);
    void retryUpload(const std::string &localPath, const std::string &serverPath, int maxRetries);
    std::string getServerTimestamp(const std::string &serverPath);
    bool isLocalFileNewer(const std::string &localPath, const std::string &serverTimestamp);

public:
    onedrive(const onedrive &) = delete;
    onedrive &operator=(const onedrive &) = delete;

    static onedrive &getInstance()
    {
        static onedrive instance;
        return instance;
    }

    void syncFiles();
};

#endif // ONEDRIVE_H
