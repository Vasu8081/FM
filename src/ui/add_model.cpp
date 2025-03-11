#include <ui/add_model.hpp>
#include <models/models_manager.hpp>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/choice.h>

AddModelForm::AddModelForm(wxWindow* parent, const std::string& type) : wxPanel(parent) {
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    inputFields.clear();
    _model = std::dynamic_pointer_cast<Model>(model_factory::create(type));
    if (!_model) {
        wxMessageBox("Invalid model type: " + type);
        return;
    }

    fieldTypes = _model->inputFormFields();
    wxFlexGridSizer* sizer = new wxFlexGridSizer(2, 10, 10);
    sizer->AddGrowableCol(1);

    for (auto& [key, fieldType] : fieldTypes) {
        wxStaticText* label = new wxStaticText(this, wxID_ANY, key + ":");
        wxControl* input = nullptr;
        std::string defaultValue = "";

        if (fieldType == "string") {
            input = new wxTextCtrl(this, wxID_ANY, defaultValue, wxDefaultPosition, wxSize(200, -1));
        } else if (fieldType == "int") {
            int value = 0;
            input = new wxSpinCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1), wxSP_ARROW_KEYS, 0, 100, value);
        } else if (fieldType == "double") {
            input = new wxTextCtrl(this, wxID_ANY, std::to_string(0.0), wxDefaultPosition, wxSize(200, -1));
        } else if (fieldType == "date") {
            wxDatePickerCtrl* datePicker = new wxDatePickerCtrl(this, wxID_ANY);
            input = datePicker;
        } else if (fieldType == "bool") {
            wxCheckBox* checkBox = new wxCheckBox(this, wxID_ANY, "");
            input = checkBox;
        } else if (fieldType == "Account") {
            model_manager& model_manager_ = model_manager::getInstance();
            auto accounts = model_manager_.getAccounts();
            wxArrayString accountChoices;
            int index = 0;
            accountChoices.Add("");
            accountMapping[index++] = "";
            for (auto& act : accounts) {
                accountChoices.Add(act.second->getName());
                accountMapping[index++] = act.first;
            }
            wxChoice* accountChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(200, -1), accountChoices);
            input = accountChoice;
        } else if (fieldType == "Category") {
            model_manager& model_manager_ = model_manager::getInstance();
            auto categories = model_manager_.getCategories();
            wxArrayString categoryChoices;
            int index = 0;
            categoryChoices.Add("");
            categoryMapping[index++] = "";
            for (auto& cat : categories) {
                categoryChoices.Add(cat.second->getName());
                categoryMapping[index++] = cat.first;
            }
            wxChoice* categoryChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(200, -1), categoryChoices);
            input = categoryChoice;
        }

        if (input) {
            inputFields[key] = input;
            sizer->Add(label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
            sizer->Add(input, 1, wxEXPAND | wxALL, 5);
        } else {
            wxMessageBox("Invalid field type: " + fieldType);
        }
    }

    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    addButton = new wxButton(this, wxID_ANY, "Add", wxDefaultPosition, wxSize(80, 30));
    addButton->Bind(wxEVT_BUTTON, &AddModelForm::OnAdd, this);
    cancelButton = new wxButton(this, wxID_ANY, "Cancel", wxDefaultPosition, wxSize(80, 30));
    cancelButton->Bind(wxEVT_BUTTON, &AddModelForm::OnCancel, this);
    buttonSizer->AddStretchSpacer();
    buttonSizer->Add(cancelButton, 0, wxRIGHT, 10);
    buttonSizer->Add(addButton, 0);

    mainSizer->Add(sizer, 1, wxEXPAND | wxALL, 10);
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 10);
    
    SetSizerAndFit(mainSizer);
    Layout();
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
            int selection = choice->GetSelection();
            if (fieldTypes[key] == "Category") {
                inputJson[key] = categoryMapping[selection];
            } else if (fieldTypes[key] == "Account") {
                inputJson[key] = accountMapping[selection];
            }
        }
    }
    
    _model->fromJson(inputJson);
    model_manager::getInstance().add(_model);
    this->GetParent()->Close();
}

void AddModelForm::OnCancel(wxCommandEvent& event) {
    this->GetParent()->Close();
}
