#ifndef APP_CONFIG_HPP
#define APP_CONFIG_HPP

#include <wx/wx.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/file.h>
#include <wx/textfile.h>
#include <json.hpp>
#include <mutex>

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
        wxFile file(_file_path.GetFullPath(), wxFile::write);
        if (!file.IsOpened())
            return;
        json j;
        toJson(j);
        wxString content = wxString::FromUTF8(j.dump(4).c_str());
        file.Write(content);
        file.Close();
    }

    // Getters
    std::string getUserName() const { return _user_name.ToStdString(); }
    std::string getAccessToken() const { return _onedrive_access_token; }
    std::string getRefreshToken() const { return _onedrive_refresh_token; }
    std::string getClientId() const { return _onedrive_client_id; }
    std::string getTenantId() const { return _onedrive_tenant_id; }
    std::string getUpstoxAccessToken() const { return _upstox_access_token; }
    wxString getAccountDefnsFilePath() const { return _account_defns_file_path.GetFullPath(); }
    wxString getTransactionsFilePath() const { return _transactions_file_path.GetFullPath(); }
    wxString getCategoriesFilePath() const { return _categories_file_path.GetFullPath(); }

    // Setters
    void setAccessToken(const std::string &access_token) { _onedrive_access_token = access_token; }
    void setRefreshToken(const std::string &refresh_token) { _onedrive_refresh_token = refresh_token; }
    void setClientId(const std::string &client_id) { _onedrive_client_id = client_id; }
    void setTenantId(const std::string &tenant_id) { _onedrive_tenant_id = tenant_id; }
    void setUpstoxAccessToken(const std::string &access_token) { _upstox_access_token = access_token; }
    void setAccountDefnsFilePath(const wxString &path) { _account_defns_file_path = wxFileName(path); }
    void setTransactionsFilePath(const wxString &path) { _transactions_file_path = wxFileName(path); }
    void setCategoriesFilePath(const wxString &path) { _categories_file_path = wxFileName(path); }

    // JSON serialization
    void fromJson(const json &j)
    {
        if (j.contains("access_token"))
            j.at("access_token").get_to(_onedrive_access_token);
        if (j.contains("refresh_token"))
            j.at("refresh_token").get_to(_onedrive_refresh_token);
        if (j.contains("client_id"))
            j.at("client_id").get_to(_onedrive_client_id);
        if (j.contains("tenant_id"))
            j.at("tenant_id").get_to(_onedrive_tenant_id);
        if (j.contains("ups_access_token"))
            j.at("ups_access_token").get_to(_upstox_access_token);
        _account_defns_file_path = wxString(j.value("account_defns_file_path", "").c_str(), wxConvUTF8);
        _transactions_file_path = wxString(j.value("transactions_file_path", "").c_str(), wxConvUTF8);
        _categories_file_path = wxString(j.value("categories_file_path", "").c_str(), wxConvUTF8);
    }

    void toJson(json &j) const
    {
        j = json{
            {"access_token", _onedrive_access_token},
            {"refresh_token", _onedrive_refresh_token},
            {"client_id", _onedrive_client_id},
            {"tenant_id", _onedrive_tenant_id},
            {"ups_access_token", _upstox_access_token},
            {"account_defns_file_path", std::string(_account_defns_file_path.GetFullPath().mb_str())},
            {"transactions_file_path", std::string(_transactions_file_path.GetFullPath().mb_str())},
            {"categories_file_path", std::string(_categories_file_path.GetFullPath().mb_str())}};
    }

private:
    wxFileName _file_path;
    std::string _onedrive_access_token;
    std::string _onedrive_refresh_token;
    std::string _onedrive_client_id;
    std::string _onedrive_tenant_id;
    std::string _upstox_access_token;
    wxFileName _account_defns_file_path;
    wxFileName _transactions_file_path;
    wxFileName _categories_file_path;
    wxString _user_name;

    void init()
    {
        _onedrive_access_token = "";
        _onedrive_refresh_token = "";
        _upstox_access_token = "";

        wxDialog dialog(NULL, wxID_ANY, "Enter client id", wxDefaultPosition, wxSize(300, 150));
        wxTextCtrl *textCtrl = new wxTextCtrl(&dialog, wxID_ANY, _onedrive_client_id, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
        wxButton *okButton = new wxButton(&dialog, wxID_OK, "OK", wxDefaultPosition, wxDefaultSize);
        wxButton *cancelButton = new wxButton(&dialog, wxID_CANCEL, "Cancel", wxDefaultPosition, wxDefaultSize);
        wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
        sizer->Add(textCtrl, 0, wxALL | wxEXPAND, 5);
        sizer->Add(okButton, 0, wxALL | wxALIGN_CENTER, 5);
        sizer->Add(cancelButton, 0, wxALL | wxALIGN_CENTER, 5);
        dialog.SetSizer(sizer);
        dialog.Fit();
        dialog.ShowModal();
        _onedrive_client_id = textCtrl->GetValue().ToStdString();

        _onedrive_tenant_id = "consumers";
        _account_defns_file_path = wxFileName();
        _transactions_file_path = wxFileName();
        _categories_file_path = wxFileName();
        _file_path.Mkdir(0777, wxPATH_MKDIR_FULL);
        save();
    }

    void load()
    {
        if (!_file_path.FileExists())
        {
            init();
            return;
        }

        wxFile file(_file_path.GetFullPath(), wxFile::read);
        if (!file.IsOpened())
        {
            std::cerr << "Error: Failed to open config file: " << _file_path.GetFullPath() << std::endl;
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

        wxDialog dialog(NULL, wxID_ANY, "Enter Username", wxDefaultPosition, wxSize(300, 150));
        wxTextCtrl *textCtrl = new wxTextCtrl(&dialog, wxID_ANY, _user_name, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
        wxButton *okButton = new wxButton(&dialog, wxID_OK, "OK", wxDefaultPosition, wxDefaultSize);
        wxButton *cancelButton = new wxButton(&dialog, wxID_CANCEL, "Cancel", wxDefaultPosition, wxDefaultSize);
        dialog.SetDefaultItem(okButton);
        wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
        sizer->Add(textCtrl, 0, wxALL | wxEXPAND, 5);
        sizer->Add(okButton, 0, wxALL | wxALIGN_CENTER, 5);
        sizer->Add(cancelButton, 0, wxALL | wxALIGN_CENTER, 5);
        dialog.SetSizer(sizer);
        
        textCtrl->Bind(wxEVT_TEXT_ENTER, [&](wxCommandEvent &) {
            wxCommandEvent event(wxEVT_BUTTON, wxID_OK);
            wxPostEvent(okButton, event);
        });        
        
        if (dialog.ShowModal() == wxID_OK) {
            _user_name = textCtrl->GetValue();
        }
        else{
            wxAbort();
        }      
    }

    // Private constructor ensures only one instance
    app_config()
    {
        wxStandardPaths &stdPaths = wxStandardPaths::Get();
        _file_path = wxFileName(stdPaths.GetUserDataDir(), "config.json");
        load();
        if (_account_defns_file_path.GetFullPath().empty())
            _account_defns_file_path = wxFileName(stdPaths.GetUserDataDir(), "account_defns.json");
        if (_transactions_file_path.GetFullPath().empty())
            _transactions_file_path = wxFileName(stdPaths.GetUserDataDir(), "transactions.json");
        if (_categories_file_path.GetFullPath().empty())
            _categories_file_path = wxFileName(stdPaths.GetUserDataDir(), "categories.json");
    }

    ~app_config()
    {
        save();
    }
};

#endif // APP_CONFIG_HPP
