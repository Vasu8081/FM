#ifndef GIVE_ACCOUNT_VIEW_HPP
#define GIVE_ACCOUNT_VIEW_HPP

#include <ui/model_view_panels/account_view.hpp>
#include <models/give_account.hpp>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/choice.h>
#include <wx/stattext.h>
#include <wx/statbmp.h>

class GiveAccountView : public AccountView
{
public:
GiveAccountView(wxWindow *parent, std::shared_ptr<GiveAccount> model) : AccountView(parent, model)
{
    // auto sizer = new wxBoxSizer(wxVERTICAL);
    
    // auto staticBox = new wxStaticBox(this, wxID_ANY, "Given Account");
    // staticBox->SetForegroundColour(_foregroundColour);
    // wxStaticBoxSizer* staticBoxSizer = new wxStaticBoxSizer(staticBox, wxVERTICAL);
    
    // auto displayFields = model->displayFormFields();
    // for (auto& [key, value] : displayFields)
    // {
    //     auto staticText = createStaticText(this, key + ": " + value);
    //     _staticTextFields[key] = staticText;
    //     staticBoxSizer->Add(staticText, 0, wxEXPAND | wxALL, 5);
    // }
    
    // auto view_transactions = new wxButton(this, wxID_ANY, "View Transactions");
    // staticBoxSizer->Add(view_transactions, 0, wxEXPAND | wxALL, 5);
    // Bind(wxEVT_BUTTON, &GiveAccountView::viewTransactions, this, view_transactions->GetId());
    
    // sizer->Add(staticBoxSizer, 0, wxEXPAND | wxALL, 10);
    
    // SetSizer(sizer);
    // SetBackgroundColour(_backgroundColour);
}

// void update() override
// {
//     auto model = std::dynamic_pointer_cast<GiveAccount>(_model);
//     auto displayFields = model->displayFormFields();
//     for (auto& [key, value] : displayFields)
//     {
//         _staticTextFields[key]->SetLabel(key + ": " + value);
//     }
// }

// private:
//     std::unordered_map<std::string, wxStaticText*> _staticTextFields;
};

#endif // BORROW_ACCOUNT_VIEW_HPP