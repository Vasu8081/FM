#include <ui/add_model.hpp>
#include <models/models_manager.hpp>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/choice.h>

AddModelForm::AddModelForm(wxWindow* parent, const std::string& type) : wxPanel(parent) {
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    inputFields.clear();
    _model = std::dynamic_pointer_cast<model>(model_factory::create(type));
    if (!_model) {
        wxMessageBox("Invalid model type: " + type);
        return;
    }

    fieldTypes = _model->fieldTypes();
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    for (auto& [key, fieldType] : fieldTypes) {
        wxStaticText* label = new wxStaticText(this, wxID_ANY, key + ":");
        wxControl* input = nullptr;
        std::string defaultValue = "";

        if (fieldType == "string") {
            input = new wxTextCtrl(this, wxID_ANY, defaultValue);
        } else if (fieldType == "int") {
            int value = 0;
            input = new wxSpinCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, value);
        } else if (fieldType == "double") {
            input = new wxTextCtrl(this, wxID_ANY, std::to_string(0.0));
        } else if (fieldType == "date") {
            wxDatePickerCtrl* datePicker = new wxDatePickerCtrl(this, wxID_ANY);
            input = datePicker;
        } else if (fieldType == "bool") {
            bool value = false;
            wxCheckBox* checkBox = new wxCheckBox(this, wxID_ANY, "");
            checkBox->SetValue(value);
            input = checkBox;
        } else if (fieldType == "account") {
            model_manager& model_manager_ = model_manager::getInstance();
            auto accounts = model_manager_.getAccounts();
            wxArrayString accountChoices;
            int index = 0;
            
            for (auto& act : accounts) {
                std::string act_id = act.first;
                std::shared_ptr<account> ac = act.second;
                accountChoices.Add(ac->getName());
                accountMapping[index++] = act_id;
            }
            
            wxChoice* accountChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, accountChoices);
            input = accountChoice;
            inputFields[key] = input;
        } else if (fieldType == "category") {
            model_manager& model_manager_ = model_manager::getInstance();
            auto categories = model_manager_.getCategories();
            wxArrayString categoryChoices;
            int index = 0;
            
            for (auto& cat : categories) {
                std::string cat_id = cat.first;
                std::shared_ptr<category> ca = cat.second;
                categoryChoices.Add(ca->getName());
                categoryMapping[index++] = cat_id;
            }
            
            wxChoice* categoryChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, categoryChoices);
            input = categoryChoice;
            inputFields[key] = input;
        }

        if (input) {
            inputFields[key] = input;
            sizer->Add(label, 0, wxALL, 5);
            sizer->Add(input, 0, wxEXPAND | wxALL, 5);
        } else {
            wxMessageBox("Invalid field type: " + fieldType);
        }
    }

    addButton = new wxButton(this, wxID_ANY, "Add");
    addButton->Bind(wxEVT_BUTTON, &AddModelForm::OnAdd, this);
    cancelButton = new wxButton(this, wxID_ANY, "Cancel");
    cancelButton->Bind(wxEVT_BUTTON, &AddModelForm::OnCancel, this);

    mainSizer->Add(sizer, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(addButton, 0, wxALIGN_CENTER | wxALL, 5);
    mainSizer->Add(cancelButton, 0, wxALIGN_CENTER | wxALL, 5);
    
    SetSizer(mainSizer);
}

bool AddModelForm::ValidateInputs() {
    return true;
}

void AddModelForm::OnAdd(wxCommandEvent& event) {
    if (!_model) return;
    
    if (!ValidateInputs()) {
        wxLogError("Validation failed. Please check the inputs.");
        return;
    }
    
    nlohmann::json inputJson;
    for (auto& [key, inputField] : inputFields) {
        if (wxTextCtrl* textCtrl = dynamic_cast<wxTextCtrl*>(inputField)) {
            inputJson[key] = textCtrl->GetValue().ToStdString();
            if (fieldTypes[key] == "int") {
                inputJson[key] = std::stoi(inputJson[key].get<std::string>());
            } else if (fieldTypes[key] == "double") {
                inputJson[key] = std::stod(inputJson[key].get<std::string>());
            }
        } else if (wxSpinCtrl* spinCtrl = dynamic_cast<wxSpinCtrl*>(inputField)) {
            inputJson[key] = static_cast<int>(spinCtrl->GetValue());
        } else if (wxDatePickerCtrl* datePicker = dynamic_cast<wxDatePickerCtrl*>(inputField)) {
            wxDateTime date = datePicker->GetValue();
            inputJson[key] = date.FormatISODate().ToStdString();
        } else if (wxCheckBox* checkBox = dynamic_cast<wxCheckBox*>(inputField)) {
            inputJson[key] = static_cast<bool>(checkBox->GetValue());
        } else if (wxChoice* choice = dynamic_cast<wxChoice*>(inputField)) {
            if ( fieldTypes[key] == "category" ) {
                int selection = choice->GetSelection();
                inputJson[key] = categoryMapping[selection];
            }
            else if ( fieldTypes[key] == "account" ) {
                int selection = choice->GetSelection();
                inputJson[key] = accountMapping[selection];
            }
        }
    }
    
    _model->fromJson(inputJson);
    model_manager::getInstance().add(_model);
}

void AddModelForm::OnCancel(wxCommandEvent& event) {
    wxLogMessage("Cancel button clicked");
    this->GetParent()->Close();
}
