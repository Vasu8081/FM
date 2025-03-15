#include <ui/edit_model.hpp>
#include <models/models_manager.hpp>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/choice.h>

EditModelForm::EditModelForm(wxWindow *parent, const std::string &type, std::shared_ptr<Model> model) 
    : wxPanel(parent), _model_type(type), _model(model)
{
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    inputFields.clear();

    if (!_model)
    {
        wxMessageBox("Invalid model type: " + type);
        return;
    }

    wxFlexGridSizer *sizer = new wxFlexGridSizer(2, 10, 10);
    sizer->AddGrowableCol(1);

    for (auto &[key, fieldType] : _model->inputFormFields())
    {
        fieldTypes[key] = fieldType;
        wxStaticText *label = new wxStaticText(this, wxID_ANY, key + ":");
        wxControl *input = nullptr;

        if (fieldType == "string")
        {
            std::string defaultValue = _model->toJson().value(key, "");
            input = new wxTextCtrl(this, wxID_ANY, defaultValue, wxDefaultPosition, wxSize(200, -1));
        }
        else if (fieldType == "int")
        {
            int value = _model->toJson().value(key, 0);
            input = new wxSpinCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1), 
                                   wxSP_ARROW_KEYS, 0, 100, value);
        }
        else if (fieldType == "double")
        {
            double value = _model->toJson().value(key, 0.0);
            input = new wxTextCtrl(this, wxID_ANY, std::to_string(value), wxDefaultPosition, wxSize(200, -1));
        }
        else if (fieldType == "date")
        {
            wxDatePickerCtrl *datePicker = new wxDatePickerCtrl(this, wxID_ANY);
            wxDateTime date;
            std::string dateStr = _model->toJson().value(key, "");
            if (!dateStr.empty())
            {
                date.ParseISODate(dateStr);
            }
            datePicker->SetValue(date);
            input = datePicker;
        }
        else if (fieldType == "bool")
        {
            bool value = _model->toJson().value(key, false);
            wxCheckBox *checkBox = new wxCheckBox(this, wxID_ANY, "");
            checkBox->SetValue(value);
            input = checkBox;
        }
        else if (fieldType == "Account")
        {
            model_manager &model_manager_ = model_manager::getInstance();
            auto accounts = model_manager_.getAccounts();
            wxArrayString accountChoices;
            int index = 0;
            accountChoices.Add("");
            accountMapping[index++] = "";
            for (auto &act : accounts)
            {
                accountChoices.Add(act.second->getName());
                accountMapping[index++] = act.first;
            }
            wxChoice *accountChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(200, -1), accountChoices);
            input = accountChoice;
        }
        else if (fieldType == "Category")
        {
            model_manager &model_manager_ = model_manager::getInstance();
            auto categories = model_manager_.getCategories();
            wxArrayString categoryChoices;
            int index = 0;
            categoryChoices.Add("");
            categoryMapping[index++] = "";
            for (auto &cat : categories)
            {
                categoryChoices.Add(cat.second->getName());
                categoryMapping[index++] = cat.first;
            }
            wxChoice *categoryChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(200, -1), categoryChoices);
            input = categoryChoice;
        }

        if (input)
        {
            inputFields[key] = input;
            input->SetMinSize(wxSize(250, -1));  // Set a minimum size for better fitting
            sizer->Add(label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
            sizer->Add(input, 1, wxEXPAND | wxALL, 5);
        }
        else
        {
            wxMessageBox("Invalid field type: " + fieldType);
        }
    }

    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    addButton = new wxButton(this, wxID_ANY, "Add", wxDefaultPosition, wxSize(80, 30));
    addButton->Bind(wxEVT_BUTTON, &EditModelForm::OnEdit, this);
    cancelButton = new wxButton(this, wxID_ANY, "Cancel", wxDefaultPosition, wxSize(80, 30));
    cancelButton->Bind(wxEVT_BUTTON, &EditModelForm::OnCancel, this);
    buttonSizer->AddStretchSpacer();
    buttonSizer->Add(cancelButton, 0, wxRIGHT, 10);
    buttonSizer->Add(addButton, 0);

    mainSizer->Add(sizer, 1, wxEXPAND | wxALL, 10);
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 10);

    SetSizer(mainSizer);
    Fit();  // Ensures proper fitting after adding elements
    Layout();  // Recalculate the layout properly
}


bool EditModelForm::ValidateInputs()
{
    return true;
}

void EditModelForm::OnEdit(wxCommandEvent &event)
{
    if (!_model)
        return;
    if (!ValidateInputs())
    {
        wxLogError("Validation failed. Please check the inputs.");
        return;
    }

    nlohmann::json inputJson;
    for (auto &[key, inputField] : inputFields)
    {
        if (wxTextCtrl *textCtrl = dynamic_cast<wxTextCtrl *>(inputField))
        {
            inputJson[key] = textCtrl->GetValue().ToStdString();
            if (fieldTypes[key] == "int")
            {
                inputJson[key] = std::stoi(inputJson[key].get<std::string>());
            }
            else if (fieldTypes[key] == "double")
            {
                inputJson[key] = std::stod(inputJson[key].get<std::string>());
            }
        }
        else if (wxSpinCtrl *spinCtrl = dynamic_cast<wxSpinCtrl *>(inputField))
        {
            inputJson[key] = static_cast<int>(spinCtrl->GetValue());
        }
        else if (wxDatePickerCtrl *datePicker = dynamic_cast<wxDatePickerCtrl *>(inputField))
        {
            wxDateTime date = datePicker->GetValue();
            inputJson[key] = date.FormatISODate().ToStdString();
        }
        else if (wxCheckBox *checkBox = dynamic_cast<wxCheckBox *>(inputField))
        {
            inputJson[key] = static_cast<bool>(checkBox->GetValue());
        }
        else if (wxChoice *choice = dynamic_cast<wxChoice *>(inputField))
        {
            int selection = choice->GetSelection();
            if (fieldTypes[key] == "Category")
            {
                inputJson[key] = categoryMapping[selection];
            }
            else if (fieldTypes[key] == "Account")
            {
                inputJson[key] = accountMapping[selection];
            }
        }
    }

    _model->fromJson(inputJson);
    _model->notifyObservers();
    this->GetParent()->Close();
}

void EditModelForm::OnCancel(wxCommandEvent &event)
{
    this->GetParent()->Close();
}
