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
    CreditCardAccountView(wxWindow *parent, std::shared_ptr<CreditCardAccount> model) : AccountView(parent, model){
        auto sizer = new wxBoxSizer(wxVERTICAL);

        wxStaticBox* staticBox = new wxStaticBox(this, wxID_ANY, "Credit Card Details");
        wxStaticBoxSizer* staticBoxSizer = new wxStaticBoxSizer(staticBox, wxVERTICAL);

        _card_number = new wxStaticText(this, wxID_ANY, "Card Number: " + model->getCardNumber());
        _cvv = new wxStaticText(this, wxID_ANY, "CVV: " + model->getCVV());
        _expiry_date = new wxStaticText(this, wxID_ANY, "Expiry Date: " + model->getExpiryDate().FormatISODate());
        _billing_date = new wxStaticText(this, wxID_ANY, "Billing Date: " + model->getBillingDate().FormatISODate());
        _payment_due_date = new wxStaticText(this, wxID_ANY, "Payment Due Date: " + model->getPaymentDueDate().FormatISODate());
        _credit_limit = new wxStaticText(this, wxID_ANY, "Credit Limit: " + std::to_string(model->getCreditLimit()));
        _limit_left = new wxStaticText(this, wxID_ANY, "Limit Left: " + std::to_string(model->getLimitLeft()));

        _card_number->SetFont(_card_number->GetFont().Bold());
        _cvv->SetFont(_cvv->GetFont().Bold());
        _expiry_date->SetFont(_expiry_date->GetFont().Bold());
        _billing_date->SetFont(_billing_date->GetFont().Bold());

        staticBoxSizer->Add(_card_number, 0, wxEXPAND | wxALL, 5);
        staticBoxSizer->Add(_cvv, 0, wxEXPAND | wxALL, 5);
        staticBoxSizer->Add(_expiry_date, 0, wxEXPAND | wxALL, 5);
        staticBoxSizer->Add(_billing_date, 0, wxEXPAND | wxALL, 5);
        staticBoxSizer->Add(_payment_due_date, 0, wxEXPAND | wxALL, 5);
        staticBoxSizer->Add(_credit_limit, 0, wxEXPAND | wxALL, 5);
        staticBoxSizer->Add(_limit_left, 0, wxEXPAND | wxALL, 5);

        auto view_transactions = new wxButton(this, wxID_ANY, "View Transactions");
        staticBoxSizer->Add(view_transactions, 0, wxEXPAND | wxALL, 5);
        Bind(wxEVT_BUTTON, &CreditCardAccountView::viewTransactions, this, view_transactions->GetId());

        sizer->Add(staticBoxSizer, 0, wxEXPAND | wxALL, 10);

        SetSizer(sizer);
        SetBackgroundColour(wxColour(254, 216, 177));
    }

    void update() override
    {
        auto model = std::dynamic_pointer_cast<CreditCardAccount>(_model);
        _limit_left->SetLabelText("Limit Left: " + std::to_string(model->getLimitLeft()));
    }
    
private:
    wxStaticText* _card_number;
    wxStaticText* _cvv;
    wxStaticText* _expiry_date;
    wxStaticText* _billing_date;
    wxStaticText* _payment_due_date;
    wxStaticText* _credit_limit;
    wxStaticText* _limit_left;
};

#endif // CREATE_CREDIT_CARD_ACCOUNT_VIEW_HPP