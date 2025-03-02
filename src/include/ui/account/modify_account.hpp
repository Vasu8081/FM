#ifndef MODIFY_ACCOUNT_HPP
#define MODIFY_ACCOUNT_HPP

#include <wx/wx.h>
#include <models/account.hpp>
#include <memory>

class ModifyAccountPanel : public wxPanel {
private:
    std::shared_ptr<account> accountData;
    wxPanel* dynamicFieldsPanel;
    wxButton* saveButton;

    void LoadAccountData();
    void OnSave(wxCommandEvent& event);

public:
    ModifyAccountPanel(wxWindow* parent, std::shared_ptr<account> acc);
};

#endif // MODIFY_ACCOUNT_HPP
