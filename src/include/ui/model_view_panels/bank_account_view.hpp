#ifndef BANK_ACCOUNT_VIEW_HPP
#define BANK_ACCOUNT_VIEW_HPP

#include <ui/model_view_panels/account_view.hpp>
#include <models/bank_account.hpp>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/choice.h>
#include <wx/stattext.h>
#include <wx/statbmp.h>
#include <wx/artprov.h>
#include <wx/dialog.h>

class BankAccountView : public AccountView
{
public:
    BankAccountView(wxWindow *parent, std::shared_ptr<bank_account> model) : AccountView(parent, model)
    {
        auto sizer = new wxBoxSizer(wxVERTICAL);
        
        wxStaticBox* staticBox = new wxStaticBox(this, wxID_ANY, "Bank Account Details");
        wxStaticBoxSizer* staticBoxSizer = new wxStaticBoxSizer(staticBox, wxVERTICAL);
        
        _account_name = new wxStaticText(this, wxID_ANY, "Bank Name: " + model->getBankName());
        _account_balance = new wxStaticText(this, wxID_ANY, "Balance: " + std::to_string(model->getBalance()));
        _account_number = new wxStaticText(this, wxID_ANY, "Account Number: " + model->getAccountNumber());
        _ifsc = new wxStaticText(this, wxID_ANY, "IFSC: " + model->getIfsc());
        _branch = new wxStaticText(this, wxID_ANY, "Branch: " + model->getBranch());
        _account_name->SetFont(_account_name->GetFont().Bold());
        _account_balance->SetFont(_account_balance->GetFont().Bold());
        
        staticBoxSizer->Add(_account_name, 0, wxEXPAND | wxALL, 5);
        staticBoxSizer->Add(_account_number, 0, wxEXPAND | wxALL, 5);
        staticBoxSizer->Add(_account_balance, 0, wxEXPAND | wxALL, 5);
        staticBoxSizer->Add(_ifsc, 0, wxEXPAND | wxALL, 5);
        staticBoxSizer->Add(_branch, 0, wxEXPAND | wxALL, 5);
        
        auto view_transactions = new wxButton(this, wxID_ANY, "View Transactions");
        staticBoxSizer->Add(view_transactions, 0, wxEXPAND | wxALL, 5);
        Bind(wxEVT_BUTTON, &BankAccountView::viewTransactions, this, view_transactions->GetId());
        
        sizer->Add(staticBoxSizer, 0, wxEXPAND | wxALL, 10);
        
        SetSizer(sizer);
        SetBackgroundColour(wxColour(240, 248, 255));
    }

    void update() override
    {
        auto model = std::dynamic_pointer_cast<bank_account>(_model);
        _account_balance->SetLabelText(std::to_string(model->getBalance()));
    }
    
private:
    wxStaticText* _account_name;
    wxStaticText* _account_number;
    wxStaticText* _account_balance;
    wxStaticText* _ifsc;
    wxStaticText* _branch;
    wxStaticText* _balance;
};

#endif // BANK_ACCOUNT_VIEW_HPP