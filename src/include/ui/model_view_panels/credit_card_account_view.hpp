#ifndef CREATE_CREDIT_CARD_ACCOUNT_VIEW_HPP
#define CREATE_CREDIT_CARD_ACCOUNT_VIEW_HPP

#include <ui/model_view_panels/account_view.hpp>
#include <models/credit_card_account.hpp>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/choice.h>
#include <wx/stattext.h>
#include <wx/statbmp.h>
#include <wx/artprov.h>
#include <wx/dialog.h>

class CreditCardAccountView : public AccountView
{
public:
    CreditCardAccountView(wxWindow *parent, std::shared_ptr<CreditCardAccount> model) : AccountView(parent, model)
    {
        auto sizer = new wxBoxSizer(wxVERTICAL);
        
        auto staticBox = new wxStaticBox(this, wxID_ANY, "Credit Card");
        staticBox->SetForegroundColour(_foregroundColour);
        wxStaticBoxSizer* staticBoxSizer = new wxStaticBoxSizer(staticBox, wxVERTICAL);
        
        auto displayFields = model->displayFormFields();
        for (auto& [key, value] : displayFields)
        {
            auto staticText = createStaticText(this, key + ": " + value);
            _staticTextFields[key] = staticText;
            staticBoxSizer->Add(staticText, 0, wxEXPAND | wxALL, 5);
        }
        
        auto view_transactions = new wxButton(this, wxID_ANY, "View Transactions");
        staticBoxSizer->Add(view_transactions, 0, wxEXPAND | wxALL, 5);
        Bind(wxEVT_BUTTON, &CreditCardAccountView::viewTransactions, this, view_transactions->GetId());
        
        sizer->Add(staticBoxSizer, 0, wxEXPAND | wxALL, 10);
        
        SetSizer(sizer);
        SetBackgroundColour(_backgroundColour);
    }
    
    void update() override
    {
        auto model = std::dynamic_pointer_cast<CreditCardAccount>(_model);
        auto displayFields = model->displayFormFields();
        for (auto& [key, value] : displayFields)
        {
            _staticTextFields[key]->SetLabel(key + ": " + value);
        }
    }
    
private:
    std::unordered_map<std::string, wxStaticText*> _staticTextFields;
};

#endif // CREATE_CREDIT_CARD_ACCOUNT_VIEW_HPP