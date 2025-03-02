#ifndef ADD_ACCOUNT_HPP
#define ADD_ACCOUNT_HPP

#include <wx/wx.h>
#include <models/account.hpp>
#include <models/account_factory.hpp>
#include <memory>

class AddAccountPanel : public wxPanel {
private:
    wxChoice* accountTypeChoice;
    wxPanel* dynamicFieldsPanel;
    wxButton* addButton;
    
    void OnAccountTypeSelected(wxCommandEvent& event);
    void OnAddAccount(wxCommandEvent& event);
    void UpdateFieldsForAccountType(const std::string& type);

public:
    AddAccountPanel(wxWindow* parent);
};

#endif // ADD_ACCOUNT_HPP