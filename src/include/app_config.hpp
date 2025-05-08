#ifndef APP_CONFIG_HPP
#define APP_CONFIG_HPP

#include <wx/wx.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/file.h>
#include <wx/textfile.h>
#include <json.hpp>
#include <mutex>
#include <sodium.h>
#include <user_config.hpp>
#include <memory>


using json = nlohmann::json;

class app_config
{
public:
    // Delete copy constructor & assignment to enforce singleton
    app_config(const app_config &) = delete;
    app_config &operator=(const app_config &) = delete;

    // Get the singleton instance (first call must provide a path)
    static app_config &getInstance()
    {
        static app_config instance;
        return instance;
    }

    void save()
    {
        wxFile file(_config_file_path.GetFullPath(), wxFile::write);
        if (!file.IsOpened())
            return;
        json j;
        toJson(j);
        wxString content = wxString::FromUTF8(j.dump(4).c_str());
        file.Write(content);
        file.Close();
    }

    // Getters
    std::string getAccessToken() const {
        if(!_current_user_config->isLoggedIn())
            return std::string();
        return _current_user_config->getAccessToken();
    }

    std::string getRefreshToken() const { 
        if(!_current_user_config->isLoggedIn())
            return std::string();
        return _current_user_config->getRefreshToken();
    }

    std::string getClientId() const { return _onedrive_client_id; }

    std::string getUpstoxAccessToken() const { 
        if(!_current_user_config->isLoggedIn())
            return std::string();
        return _current_user_config->getUpstoxAccessToken();    
    }

    wxString getAccountDefnsFilePath() const {
        if(!_current_user_config->isLoggedIn())
            return wxString();
        return _current_user_config->getAccountDefnsFilePath(); 
    }

    wxString getTransactionsFilePath() const {
        if(!_current_user_config->isLoggedIn())
            return wxString();
        return _current_user_config->getTransactionsFilePath(); 
    }

    wxString getCategoriesFilePath() const {
        if(!_current_user_config->isLoggedIn())
            return wxString();
        return _current_user_config->getCategoriesFilePath(); 
    }

    wxString getUserFolderPath() const {
        if(!_current_user_config->isLoggedIn())
            return wxString();
        return _current_user_config->getUserFolderPath(); 
    }


    // Setters
    void setAccessToken(const std::string &access_token) {
        if (!_current_user_config->isLoggedIn())
            return;
        _current_user_config->setOnedriveAccessToken(access_token);
    }

    void setRefreshToken(const std::string &refresh_token) {
        if (!_current_user_config->isLoggedIn())
            return;
        _current_user_config->setOnedriveRefreshToken(refresh_token);
    }

    void setClientId(const std::string &client_id) { _onedrive_client_id = client_id; }

    void setUpstoxAccessToken(const std::string &access_token) {
        if (!_current_user_config->isLoggedIn())
            return;
        _current_user_config->setUpstoxAccessToken(access_token);
    }

    // JSON serialization
    void fromJson(const json &j)
    {
        if (j.contains("onedrive_client_id"))
            j.at("onedrive_client_id").get_to(_onedrive_client_id);
        
        for (auto &user_entry : j["user_configs"])
        {
            std::string user_name = user_entry["user_name"].get<std::string>();
            auto user_config_ptr = std::make_shared<user_config>(user_name);
            user_config_ptr->load(user_entry);
            _user_configs[user_name] = user_config_ptr;
        }
    }
    

    void toJson(json &j) const
    {
        j["user_configs"] = json::array();
        j["onedrive_client_id"] = _onedrive_client_id;
        for (const auto &user_config_pair : _user_configs)
        {
            j["user_configs"].push_back(user_config_pair.second->toJson());
        }
    }

private:
    wxFileName _config_file_path;
    std::string _onedrive_client_id;
    std::shared_ptr<user_config> _current_user_config;
    std::map<std::string, std::shared_ptr<user_config>> _user_configs;


    void init()
    {
        _config_file_path.Mkdir(0777, wxPATH_MKDIR_FULL);
        save();
    }

    void load()
    {
        if (!_config_file_path.FileExists())
        {
            init();
            return;
        }
    
        wxFile file(_config_file_path.GetFullPath(), wxFile::read);
        if (!file.IsOpened())
        {
            std::cerr << "Error: Failed to open config file: " << _config_file_path.GetFullPath() << std::endl;
            return;
        }
    
        wxString content;
        file.ReadAll(&content);
        file.Close();
    
        try
        {
            json j = json::parse(std::string(content.mb_str()));
            fromJson(j);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error parsing JSON: " << e.what() << std::endl;
        }
    
        wxDialog dialog(nullptr, wxID_ANY, "Login Dialog", wxDefaultPosition, wxSize(300, 200));
        
        wxTextCtrl *username_input = new wxTextCtrl(&dialog, wxID_ANY, "", wxDefaultPosition, wxSize(200, 25));
        wxTextCtrl *password_input = new wxTextCtrl(&dialog, wxID_ANY, "", wxDefaultPosition, wxSize(200, 25), wxTE_PASSWORD);
        
        wxButton *login_button = new wxButton(&dialog, wxID_OK, "Login", wxDefaultPosition, wxSize(100, 25));
        wxButton *cancel_button = new wxButton(&dialog, wxID_CANCEL, "Cancel", wxDefaultPosition, wxSize(100, 25));
        wxButton *register_button = new wxButton(&dialog, wxID_ANY, "Register", wxDefaultPosition, wxSize(100, 25)); // ✅ Fixed Pointer Issue
    
        register_button->Bind(wxEVT_BUTTON, [&](wxCommandEvent &event) {
            std::string username = username_input->GetValue().ToStdString();
            std::string password = password_input->GetValue().ToStdString();
            
            auto user_config_ptr = std::make_shared<user_config>(username);
            if (user_config_ptr->createUser(username, password)) // Only update if creation succeeds
            {
                _current_user_config = user_config_ptr; // ✅ Use shared_ptr instead of copying
                _user_configs[username] = user_config_ptr;
                save();
                dialog.EndModal(wxID_OK);
            }
            else
            {
                wxMessageBox("User registration failed", "Error", wxOK | wxICON_ERROR, &dialog);
            }
        });
    
        login_button->Bind(wxEVT_BUTTON, [&](wxCommandEvent &event) {
            std::string username = username_input->GetValue().ToStdString();
            std::string password = password_input->GetValue().ToStdString();
            
            auto it = _user_configs.find(username);
            if (it != _user_configs.end())
            {
                if (it->second->verifyLogin(password)) // ✅ Fixed missing parameter
                {
                    _current_user_config = it->second; // ✅ Use shared_ptr assignment
                    save();
                    dialog.EndModal(wxID_OK);
                }
                else
                {
                    wxMessageBox("Invalid password", "Error", wxOK | wxICON_ERROR, &dialog);
                }
            }
            else
            {
                wxMessageBox("User not found", "Error", wxOK | wxICON_ERROR, &dialog);
            }
        });
    
        cancel_button->Bind(wxEVT_BUTTON, [&](wxCommandEvent &event) {
            dialog.EndModal(wxID_CANCEL);
        });
    
        wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
        sizer->Add(username_input, 0, wxALL | wxEXPAND, 5);
        sizer->Add(password_input, 0, wxALL | wxEXPAND, 5);
        sizer->Add(login_button, 0, wxALL | wxALIGN_CENTER, 5);
        sizer->Add(cancel_button, 0, wxALL | wxALIGN_CENTER, 5);
        sizer->Add(register_button, 0, wxALL | wxALIGN_CENTER, 5);
    
        dialog.SetSizer(sizer);
        dialog.Layout();
        dialog.Centre(wxBOTH);
        dialog.ShowModal();
    }
    
    // Private constructor ensures only one instance
    app_config()
    {
        wxStandardPaths &stdPaths = wxStandardPaths::Get();
        _config_file_path = wxFileName(stdPaths.GetUserDataDir(), "config.json");
        load();
    }

    ~app_config()
    {
        save();
    }
};

#endif // APP_CONFIG_HPP
