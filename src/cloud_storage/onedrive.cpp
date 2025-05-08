#include <cloud_storage/onedrive.hpp>
#include <crow.h>
#include <fstream>
#include <thread>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <ctime>
#include <wx/datetime.h>

using json = nlohmann::json;
namespace fs = std::filesystem;

std::string auth_code;
bool auth_received = false;

void onedrive::startLocalServer() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/callback")
    ([&](const crow::request& req) {
        auto query_params = crow::query_string(req.url_params);
        if (query_params.get("code")) {
            auth_code = query_params.get("code");
            auth_received = true;
            return "Authorization successful! You can close this window.";
        }
        return "Failed to retrieve authorization code.";
    });

    std::thread server_thread([&]() {
        app.port(8080).multithreaded().run();
    });

    while (!auth_received) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    app.stop();
    server_thread.join();
}

std::string onedrive::waitForAuthCode()
{
    std::string auth_url = "https://login.microsoftonline.com/consumers/oauth2/v2.0/authorize?client_id=" + _config.getClientId() +
    "&response_type=code" +
    "&redirect_uri=http://localhost:8080/callback" +
    "&scope=Files.ReadWrite offline_access" +
    "&prompt=select_account";

    std::string command;

    #ifdef _WIN32
        command = "start \"\" \"" + auth_url + "\"";  // Windows
    #elif __APPLE__
        command = "open \"" + auth_url + "\"";  // macOS
    #else
        command = "xdg-open \"" + auth_url + "\"";  // Linux
    #endif

    wxMessageBox("Please authorize the application by clicking on the link in the browser.", "Authorization Required", wxOK | wxICON_INFORMATION);

    system(command.c_str());
    startLocalServer();

    int maxWait = 60; // Maximum wait time in seconds
    while (!auth_received && maxWait > 0)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        maxWait--;
    }

    return auth_code;
}

std::string onedrive::getAccessToken(const std::string &authCode)
{
    CURL *curl = curl_easy_init();
    if (!curl)
        return "";

    std::string url = "https://login.microsoftonline.com/consumers/oauth2/v2.0/token";
    std::string postFields = "client_id=" + _config.getClientId() +
                             "&code=" + authCode +
                             "&redirect_uri=http://localhost:8080/callback"
                             "&grant_type=authorization_code";

    std::string response;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK)
        return "";

    json responseJson = json::parse(response);
    if (responseJson.contains("access_token"))
    {
        _config.setAccessToken(responseJson["access_token"]);
        _config.setRefreshToken(responseJson["refresh_token"]);
        _config.save();
        return responseJson["access_token"];
    }
    return "";
}

// Curl response write callback
size_t onedrive::writeCallback(void *contents, size_t size, size_t nmemb, std::string *userData)
{
    size_t totalSize = size * nmemb;
    userData->append((char *)contents, totalSize);
    return totalSize;
}

// Curl write callback for file downloads
size_t onedrive::writeFileCallback(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    return fwrite(ptr, size, nmemb, stream);
}

// Refresh token if available
std::string onedrive::refreshAccessToken()
{
    CURL *curl = curl_easy_init();
    if (!curl)
        return "";

    std::string url = "https://login.microsoftonline.com/consumers/oauth2/v2.0/token";
    std::string postFields =
        "grant_type=refresh_token"
        "&client_id=" +
        _config.getClientId() +
        "&refresh_token=" + _config.getRefreshToken();

    std::string response;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK)
        return "";

    json responseJson = json::parse(response);
    if (responseJson.contains("access_token"))
    {
        _config.setAccessToken(responseJson["access_token"]);
        _config.setRefreshToken(responseJson["refresh_token"]);
        _config.save();
        return responseJson["access_token"];
    }

    return "";
}

// Upload file to onedrive
bool onedrive::uploadFile(const std::string &localPath, const std::string &oneDrivePath)
{
    std::ifstream file(localPath, std::ios::binary);
    if (!file.is_open())
    {
        return false;
    }

    std::string fileData((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    std::string url = "https://graph.microsoft.com/v1.0/me/drive/root:/" + oneDrivePath + ":/content";

    CURL *curl = curl_easy_init();
    if (!curl)
    {
        return false;
    }

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, ("Authorization: Bearer " + _config.getAccessToken()).c_str());
    headers = curl_slist_append(headers, "Content-Type: application/octet-stream");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, fileData.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    CURLcode res = curl_easy_perform(curl);
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    if (res != CURLE_OK)
    {
        return false;
    }
    else if (http_code != 200 && http_code != 201)
    {
        return false;
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    return true;
}

// Download file from onedrive
bool onedrive::downloadFile(const std::string &oneDrivePath, const std::string &localPath)
{
    std::string url = "https://graph.microsoft.com/v1.0/me/drive/root:/" + oneDrivePath + ":/content";

    std::filesystem::create_directories(std::filesystem::path(localPath).parent_path());

    FILE *file = fopen(localPath.c_str(), "wb");
    if (!file)
    {
        std::cerr << "Error: Could not open file for writing: " << localPath << std::endl;
        return false;
    }

    CURL *curl = curl_easy_init();
    if (!curl)
    {
        std::cerr << "Error: Failed to initialize curl for download" << std::endl;
        fclose(file);
        return false;
    }

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, ("Authorization: Bearer " + _config.getAccessToken()).c_str());

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFileCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

    // Tell libcurl to follow redirects (fixes HTTP 302 error)
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    CURLcode res = curl_easy_perform(curl);
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    if (res != CURLE_OK)
    {
        std::cerr << "Download failed: " << curl_easy_strerror(res) << std::endl;
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        fclose(file);
        return false;
    }
    else if (http_code == 200)
    {
        std::cout << "File downloaded successfully from onedrive: " << oneDrivePath << std::endl;
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        fclose(file);
        return true;
    }
    else
    {
        std::cerr << "Download failed! HTTP Response Code: " << http_code << std::endl;
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        fclose(file);
        return false;
    }
}

bool onedrive::isLocalFileNewer(const std::string &localFile, const std::string &serverTimestamp)
{
    if (!fs::exists(localFile))
        return false;

    auto lastModified = fs::last_write_time(localFile);
    wxDateTime localTime;
    auto sysTime = std::chrono::clock_cast<std::chrono::system_clock>(lastModified);
    localTime.Set((time_t)std::chrono::duration_cast<std::chrono::seconds>(
        sysTime.time_since_epoch()).count());
    

    wxDateTime serverTime;
    serverTime.ParseISOCombined(serverTimestamp, 'T');

    // wxMessageBox("Local Time: " + localTime.ToUTC().FormatISOCombined('T') + "\nServer Time: " + serverTime.FormatISOCombined('T'), "Time Comparison", wxOK | wxICON_INFORMATION);

    return localTime.ToUTC().IsLaterThan(serverTime);
}


std::string onedrive::getServerTimestamp(const std::string &serverPath)
{
    CURL *curl = curl_easy_init();
    if (!curl)
        return "";

    std::string url = "https://graph.microsoft.com/v1.0/me/drive/root:/" + serverPath + "?select=lastModifiedDateTime";
    std::string response;
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, ("Authorization: Bearer " + _config.getAccessToken()).c_str());

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    if (res != CURLE_OK)
        return "";

    if(http_code == 404){
        return "Not Found";
    }
    else if (http_code != 200)
        return "";
    
    json responseJson = json::parse(response);
    if (responseJson.contains("lastModifiedDateTime"))
        return responseJson["lastModifiedDateTime"];

    return "";
}

void onedrive::retryUpload(const std::string &localPath, const std::string &serverPath, int maxRetries)
{
    int retries = 0;
    while (retries < maxRetries)
    {
        if (uploadFile(localPath, serverPath))
            return;
        std::cerr << "Upload failed, retrying... (Attempt " << (retries + 1) << "/" << maxRetries << ")\n";
        std::this_thread::sleep_for(std::chrono::seconds(2));
        retries++;
    }
    std::cerr << "Upload failed after " << maxRetries << " attempts." << std::endl;
}

void onedrive::retryDownload(const std::string &serverPath, const std::string &localPath, int maxRetries)
{
    int retries = 0;
    while (retries < maxRetries)
    {
        if (downloadFile(serverPath, localPath))
            return;
        std::cerr << "Download failed, retrying... (Attempt " << (retries + 1) << "/" << maxRetries << ")\n";
        std::this_thread::sleep_for(std::chrono::seconds(2));
        retries++;
    }
    std::cerr << "Download failed after " << maxRetries << " attempts." << std::endl;
}

void onedrive::syncFiles()
{
    std::vector<std::string> files = {"transactions.json", "account_defns.json", "categories.json"};
    
    for (const auto &file : files)
    {
        std::string localPath = _config.getUserFolderPath().ToStdString()+"/" + file;
        std::string serverPath = "FinanceManager/" + file;
        
        std::string serverTimestamp = getServerTimestamp(serverPath);
        if(serverTimestamp == "Not Found" && fs::exists(localPath)){
            retryUpload(localPath, serverPath, 3);
            continue;
        }
        else if(serverTimestamp == "Not Found" && !fs::exists(localPath)){
            std::ofstream file(localPath);
            file << "{}";
            file.close();
            retryUpload(localPath, serverPath, 3);
            continue;
        }
        if (isLocalFileNewer(localPath, serverTimestamp))
        {
            retryUpload(localPath, serverPath, 3);
        }
        else
        {
            retryDownload(serverPath, localPath, 3);
        }
    }
}

