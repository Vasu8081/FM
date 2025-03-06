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

    wxStaticText* createStaticText(wxWindow* parent, const wxString& label)
    {
        auto staticText = new wxStaticText(parent, wxID_ANY, label);
        staticText->SetForegroundColour(wxColour(221, 221, 221));
        return staticText;
    }

    BankAccountView(wxWindow *parent, std::shared_ptr<BankAccount> model) : AccountView(parent, model)
    {
        auto sizer = new wxBoxSizer(wxVERTICAL);
        auto foreGroundColour = wxColour(221, 221, 221);
        auto backGroundColour = wxColour(34, 40, 49);
        
        auto staticBox = new wxStaticBox(this, wxID_ANY, "Bank Account");
        staticBox->SetForegroundColour(foreGroundColour);
        wxStaticBoxSizer* staticBoxSizer = new wxStaticBoxSizer(staticBox, wxVERTICAL);
        
        _header = createStaticText(this, model->getHeader());
        _header->SetFont(_header->GetFont().Bold());
        _account_number = new wxStaticText(this, wxID_ANY, "Account Number: " + model->getAccountNumber());
        _account_number->SetForegroundColour(wxColour(221, 221, 221));
        _ifsc = new wxStaticText(this, wxID_ANY, "IFSC: " + model->getIfsc());
        _ifsc->SetForegroundColour(wxColour(221, 221, 221));
        _branch = new wxStaticText(this, wxID_ANY, "Branch: " + model->getBranch());
        _branch->SetForegroundColour(wxColour(221, 221, 221));
        
        staticBoxSizer->Add(_header, 0, wxEXPAND | wxALL, 5);
        staticBoxSizer->Add(_account_number, 0, wxEXPAND | wxALL, 5);
        staticBoxSizer->Add(_ifsc, 0, wxEXPAND | wxALL, 5);
        staticBoxSizer->Add(_branch, 0, wxEXPAND | wxALL, 5);
        
        auto view_transactions = new wxButton(this, wxID_ANY, "View Transactions");
        staticBoxSizer->Add(view_transactions, 0, wxEXPAND | wxALL, 5);
        Bind(wxEVT_BUTTON, &BankAccountView::viewTransactions, this, view_transactions->GetId());
        
        sizer->Add(staticBoxSizer, 0, wxEXPAND | wxALL, 10);
        
        SetSizer(sizer);
        SetBackgroundColour(wxColour(34, 40, 49));
    }

    void update() override
    {
        auto model = std::dynamic_pointer_cast<BankAccount>(_model);
        _header->SetLabelText(model->getHeader());
    }
    
private:
    wxStaticText* _header;
    wxStaticText* _account_number;
    wxStaticText* _ifsc;
    wxStaticText* _branch;
    wxStaticText* _balance;
};

#endif // BANK_ACCOUNT_VIEW_HPP