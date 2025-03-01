#ifndef ONEDRIVE_H
#define ONEDRIVE_H

#include <iostream>
#include <string>
#include <curl/curl.h>
#include <json.hpp>
#include <app_config.hpp>

class onedrive {
private:
    app_config& _config = app_config::getInstance();

    // Helper functions
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* userData);
    static size_t writeFileCallback(void* ptr, size_t size, size_t nmemb, FILE* stream);
    
    std::string getDeviceCode(std::string &userCode, int &interval);
    std::string getAccessToken(const std::string& deviceCode, int interval);
    std::string refreshAccessToken();
    void uploadFile(const std::string& localPath, const std::string& oneDrivePath);
    void downloadFile(const std::string& oneDrivePath, const std::string& localPath);

public:
    onedrive();
    void sync_up();
    void sync_down();
};

#endif // ONEDRIVE_H
