#include <cloud_storage/onedrive.hpp>
#include <crow.h>
#include <fstream>
#include <thread>
#include <chrono>

using json = nlohmann::json;

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
    "&response_type=code&redirect_uri=http://localhost:8080/callback&scope=Files.ReadWrite offline_access";

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

    std::string url = "https://login.microsoftonline.com/" + _config.getTenantId() + "/oauth2/v2.0/token";
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
void onedrive::uploadFile(const std::string &localPath, const std::string &oneDrivePath)
{
    std::ifstream file(localPath, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file for upload: " << localPath << std::endl;
        return;
    }

    std::string fileData((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    std::string url = "https://graph.microsoft.com/v1.0/me/drive/root:/" + oneDrivePath + ":/content";
    std::cout << "🔹 Uploading to: " << url << std::endl;

    CURL *curl = curl_easy_init();
    if (!curl)
    {
        std::cerr << "Error: Failed to initialize curl for upload" << std::endl;
        return;
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
        std::cerr << "Upload failed: " << curl_easy_strerror(res) << std::endl;
    }
    else if (http_code != 200 && http_code != 201)
    {
        std::cerr << "Upload failed! HTTP Response Code: " << http_code << std::endl;
    }
    else
    {
        std::cout << "File uploaded successfully to onedrive: " << oneDrivePath << std::endl;
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
}

// Download file from onedrive
void onedrive::downloadFile(const std::string &oneDrivePath, const std::string &localPath)
{
    std::string url = "https://graph.microsoft.com/v1.0/me/drive/root:/" + oneDrivePath + ":/content";
    std::cout << "🔹 Downloading from: " << url << std::endl;

    FILE *file = fopen(localPath.c_str(), "wb");
    if (!file)
    {
        std::cerr << "Error: Could not open file for writing: " << localPath << std::endl;
        return;
    }

    CURL *curl = curl_easy_init();
    if (!curl)
    {
        std::cerr << "Error: Failed to initialize curl for download" << std::endl;
        fclose(file);
        return;
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
    }
    else if (http_code == 200)
    {
        std::cout << "File downloaded successfully from onedrive: " << oneDrivePath << std::endl;
    }
    else
    {
        std::cerr << "Download failed! HTTP Response Code: " << http_code << std::endl;
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    fclose(file);
}

void onedrive::sync_up()
{
    uploadFile(_config.getTransactionsFilePath().ToStdString(), "FinanceManager/" + _config.getUserName() + "/transactions.json");
    uploadFile(_config.getAccountDefnsFilePath().ToStdString(), "FinanceManager/" + _config.getUserName() + "/account_defns.json");
    uploadFile(_config.getCategoriesFilePath().ToStdString(), "FinanceManager/" + _config.getUserName() + "/categories.json");
}

void onedrive::sync_down()
{
    downloadFile("FinanceManager/" + _config.getUserName() + "/transactions.json", _config.getTransactionsFilePath().ToStdString());
    downloadFile("FinanceManager/" + _config.getUserName() + "/account_defns.json", _config.getAccountDefnsFilePath().ToStdString());
    downloadFile("FinanceManager/" + _config.getUserName() + "/categories.json", _config.getCategoriesFilePath().ToStdString());
}
