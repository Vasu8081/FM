#ifndef USER_CONFIG_HPP
#define USER_CONFIG_HPP

#include <string>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <json.hpp>
#include <sodium.h>

using json = nlohmann::json;

class user_config {
public:
    // Default constructor
    user_config() = default;

    // Constructor with username to set up paths
    explicit user_config(const std::string& username) : _user_name(username) {
        wxString stdPath = wxStandardPaths::Get().GetUserDataDir();
        _userFolderPath = wxFileName(stdPath + "/" + _user_name, "");
    }

    // Getters
    std::string getUserName() const { return _user_name; }
    bool isLoggedIn() const { return _isLoggedIn; }

    std::string getAccessToken() const { return _onedrive_access_token; }

    std::string getRefreshToken() const { return _onedrive_refresh_token; }

    std::string getUpstoxAccessToken() const { return _upstox_access_token; }

    wxString getAccountDefnsFilePath() const {
        return wxFileName(_userFolderPath.GetFullPath(), "account_defns.json").GetFullPath();
    }

    wxString getTransactionsFilePath() const {
        return wxFileName(_userFolderPath.GetFullPath(), "transactions.json").GetFullPath();
    }

    wxString getCategoriesFilePath() const {
        return wxFileName(_userFolderPath.GetFullPath(), "categories.json").GetFullPath();
    }

    wxString getUserFolderPath() const {
        return _userFolderPath.GetFullPath();
    }

    // Load data from JSON
    void load(const json& j) {
        if (j.contains("user_name")) j.at("user_name").get_to(_user_name);
        if (j.contains("password")) j.at("password").get_to(_hashed_password);
        if (j.contains("access_token")) j.at("access_token").get_to(_onedrive_access_token);
        if (j.contains("refresh_token")) j.at("refresh_token").get_to(_onedrive_refresh_token);
        if (j.contains("ups_access_token")) j.at("ups_access_token").get_to(_upstox_access_token);
    }

    // Convert to JSON
    json toJson() const {
        json j;
        j["user_name"] = _user_name;
        j["password"] = _hashed_password;
        j["access_token"] = _onedrive_access_token;
        j["refresh_token"] = _onedrive_refresh_token;
        j["ups_access_token"] = _upstox_access_token;
        return j;
    }

    // Create user and hash password
    bool createUser(const std::string& username, const std::string& password) {
        if (username.empty() || password.empty()) return false;

        unsigned char hashed[crypto_pwhash_STRBYTES];
        if (crypto_pwhash_str(reinterpret_cast<char*>(hashed), password.c_str(), password.length(),
                              crypto_pwhash_OPSLIMIT_MODERATE, crypto_pwhash_MEMLIMIT_MODERATE) != 0) {
            return false;
        }

        _user_name = username;
        _hashed_password = std::string(reinterpret_cast<char*>(hashed));
        _isLoggedIn = true;

        return true;
    }

    // Verify login credentials
    bool verifyLogin(const std::string& password) {
        // if (_hashed_password.empty()) return false;

        if (crypto_pwhash_str_verify(_hashed_password.c_str(), password.c_str(), password.length()) == 0) {
            _isLoggedIn = true;
            return true;
        }

        return false;
    }

    // Setters for access tokens
    void setOnedriveAccessToken(const std::string& access_token) {
        _onedrive_access_token = access_token;
    }

    void setOnedriveRefreshToken(const std::string& refresh_token) {
        _onedrive_refresh_token = refresh_token;
    }

    void setUpstoxAccessToken(const std::string& access_token) {
        _upstox_access_token = access_token;
    }

private:
    std::string _user_name;
    std::string _hashed_password;
    std::string _onedrive_access_token;
    std::string _onedrive_refresh_token;
    std::string _upstox_access_token;
    wxFileName _userFolderPath;
    bool _isLoggedIn = false;
};

#endif // USER_CONFIG_HPP
