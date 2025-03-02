#ifndef ADD_ACCOUNT_HPP
#define ADD_ACCOUNT_HPP

#include <wx/wx.h>
#include <wx/datectrl.h>
#include <models/account.hpp>
#include <models/account_factory.hpp>
#include <json.hpp>
#include <memory>
#include <map>

class AddAccountPanel : public wxPanel {
private:
    wxChoice* accountTypeChoice;
    wxPanel* dynamicFieldsPanel;
    wxButton* addButton;
    std::shared_ptr<account> currentAccount;
    std::unordered_map<std::string, wxControl*> inputFields;
    std::unordered_map<std::string, std::string> fieldTypes;
    
    void OnAccountTypeSelected(wxCommandEvent& event);
    void OnAddAccount(wxCommandEvent& event);
    void UpdateFieldsForAccountType(const std::string& type);
    bool ValidateInputs();

public:
    AddAccountPanel(wxWindow* parent);
};

#endif // ADD_ACCOUNT_HPP