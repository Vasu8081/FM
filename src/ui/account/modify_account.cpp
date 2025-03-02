#include <ui/account/modify_account.hpp>
#include <wx/sizer.h>

ModifyAccountPanel::ModifyAccountPanel(wxWindow* parent, std::shared_ptr<account> acc) : wxPanel(parent), accountData(acc) {
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    
    dynamicFieldsPanel = new wxPanel(this);
    LoadAccountData();
    
    saveButton = new wxButton(this, wxID_ANY, "Save Changes");
    saveButton->Bind(wxEVT_BUTTON, &ModifyAccountPanel::OnSave, this);
    
    mainSizer->Add(dynamicFieldsPanel, 1, wxEXPAND | wxALL, 5);
    mainSizer->Add(saveButton, 0, wxALIGN_CENTER | wxALL, 5);
    
    SetSizer(mainSizer);
}

void ModifyAccountPanel::LoadAccountData() {
    dynamicFieldsPanel->DestroyChildren();
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    
    sizer->Add(new wxStaticText(dynamicFieldsPanel, wxID_ANY, "Account Name:"), 0, wxALL, 5);
    sizer->Add(new wxTextCtrl(dynamicFieldsPanel, wxID_ANY, accountData->getName()), 0, wxEXPAND | wxALL, 5);
    
    dynamicFieldsPanel->SetSizer(sizer);
    dynamicFieldsPanel->Layout();
}

void ModifyAccountPanel::OnSave(wxCommandEvent& event) {
    wxLogMessage("Account Updated!");
}