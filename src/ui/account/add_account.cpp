#include <ui/account/add_account.hpp>
#include <models/models_manager.hpp>
#include <wx/sizer.h>

AddAccountPanel::AddAccountPanel(wxWindow* parent) : wxPanel(parent) {
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    
    wxArrayString accountTypes;
    accountTypes.Add("Bank Account");
    accountTypes.Add("Loan Account");
    accountTypes.Add("Investment Account");
    
    accountTypeChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, accountTypes);
    accountTypeChoice->Bind(wxEVT_CHOICE, &AddAccountPanel::OnAccountTypeSelected, this);
    
    dynamicFieldsPanel = new wxPanel(this);
    
    addButton = new wxButton(this, wxID_ANY, "Add Account");
    addButton->Bind(wxEVT_BUTTON, &AddAccountPanel::OnAddAccount, this);
    
    mainSizer->Add(accountTypeChoice, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(dynamicFieldsPanel, 1, wxEXPAND | wxALL, 5);
    mainSizer->Add(addButton, 0, wxALIGN_CENTER | wxALL, 5);
    
    SetSizer(mainSizer);
}

void AddAccountPanel::OnAccountTypeSelected(wxCommandEvent& event) {
    wxString selectedType = accountTypeChoice->GetStringSelection();
    UpdateFieldsForAccountType(std::string(selectedType.mb_str()));
}

void AddAccountPanel::UpdateFieldsForAccountType(const std::string& type) {
    dynamicFieldsPanel->DestroyChildren();
    
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    if (type == "Bank Account") {
        sizer->Add(new wxStaticText(dynamicFieldsPanel, wxID_ANY, "Bank Name:"), 0, wxALL, 5);
        sizer->Add(new wxTextCtrl(dynamicFieldsPanel, wxID_ANY), 0, wxEXPAND | wxALL, 5);
    }
    dynamicFieldsPanel->SetSizer(sizer);
    dynamicFieldsPanel->Layout();
}

void AddAccountPanel::OnAddAccount(wxCommandEvent& event) {
    wxLogMessage("New Account Added!");
}