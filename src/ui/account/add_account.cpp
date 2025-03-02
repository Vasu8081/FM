#include <ui/account/add_account.hpp>
#include <models/models_manager.hpp>
#include <wx/sizer.h>
#include <wx/spinctrl.h>


AddAccountPanel::AddAccountPanel(wxWindow* parent) : wxPanel(parent) {
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    
    wxArrayString accountTypes;
    accountTypes.Add("Bank Account");
    
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
    inputFields.clear();
    
    currentAccount = account_factory::createAccount(type);
    if (!currentAccount) return;
    
    fieldTypes = currentAccount->fieldTypes();
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    
    for (auto& [key, fieldType] : fieldTypes) {
        wxStaticText* label = new wxStaticText(dynamicFieldsPanel, wxID_ANY, key + ":");
        wxControl* input = nullptr;
        
        if (fieldType == "string") {
            input = new wxTextCtrl(dynamicFieldsPanel, wxID_ANY, "Default");
        } else if (fieldType == "int") {
            input = new wxSpinCtrl(dynamicFieldsPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0);
        } else if (fieldType == "double") {
            input = new wxTextCtrl(dynamicFieldsPanel, wxID_ANY, "0.0");
        } else if (fieldType == "date") {
            input = new wxDatePickerCtrl(dynamicFieldsPanel, wxID_ANY);
        }
        
        if (input) {
            inputFields[key] = input;
            sizer->Add(label, 0, wxALL, 5);
            sizer->Add(input, 0, wxEXPAND | wxALL, 5);
        }
    }
    
    dynamicFieldsPanel->SetSizer(sizer);
    dynamicFieldsPanel->Layout();
}

bool AddAccountPanel::ValidateInputs() {
    return true;
}

void AddAccountPanel::OnAddAccount(wxCommandEvent& event) {
    if (!currentAccount) return;
    
    if (!ValidateInputs()) {
        wxLogError("Validation failed. Please check the inputs.");
        return;
    }
    
    nlohmann::json inputJson;
    for (auto& [key, inputField] : inputFields) {
        if (wxTextCtrl* textCtrl = dynamic_cast<wxTextCtrl*>(inputField)) {
            inputJson[key] = std::string(textCtrl->GetValue().mb_str());
        } else if (wxSpinCtrl* spinCtrl = dynamic_cast<wxSpinCtrl*>(inputField)) {
            inputJson[key] = spinCtrl->GetValue();
        } else if (wxDatePickerCtrl* datePicker = dynamic_cast<wxDatePickerCtrl*>(inputField)) {
            wxDateTime date = datePicker->GetValue();
            inputJson[key] = date.FormatISODate().ToStdString();
        }
    }
    
    currentAccount->from_json(inputJson);
    model_manager::getInstance().addAccount(currentAccount);
    wxLogMessage("New Account Added!");
}
