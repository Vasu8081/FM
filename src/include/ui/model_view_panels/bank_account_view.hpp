#ifndef BANK_ACCOUNT_VIEW_HPP
#define BANK_ACCOUNT_VIEW_HPP

#include <ui/model_view_panels/model_view.hpp>
#include <models/bank_account.hpp>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/choice.h>
#include <wx/stattext.h>
#include <wx/statbmp.h>
#include <wx/artprov.h>
#include <wx/dialog.h>

class BankAccountView : public ModelView
{
public:
    BankAccountView(wxWindow *parent, std::shared_ptr<bank_account> model) : ModelView(parent, model)
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

    void viewTransactions(wxCommandEvent& event){
        auto model = std::dynamic_pointer_cast<bank_account>(_model);
        auto from_transactions = model->getFromTransactions();
        auto to_transactions = model->getToTransactions();
        auto transactions = from_transactions;
        transactions.insert(transactions.end(), to_transactions.begin(), to_transactions.end());
        
        std::sort(transactions.begin(), transactions.end(), [](std::shared_ptr<transaction> a, std::shared_ptr<transaction> b){
            return a->getDate() > b->getDate();
        });
        
        auto dialog = new wxDialog(this, wxID_ANY, "Transactions", wxDefaultPosition, wxSize(400, 600), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
        auto sizer = new wxBoxSizer(wxVERTICAL);
        
        for(auto& t : transactions){
            wxBoxSizer* transactionSizer = new wxBoxSizer(wxHORIZONTAL);

            bool isOutgoing = (t->getFromAccount() && t->getFromAccount()->generateID() == model->generateID());
            wxBitmap transactionIcon = isOutgoing ? _icon.get(wxART_CALL_MADE) : _icon.get(wxART_CALL_RECEIVED);
            
            wxBitmap forwardIcon = _icon.get(wxART_ARROW_FORWARD);
            wxBitmap rupeeIcon = _icon.get(wxART_CURRENCY_RUPEE);
            wxBitmap categoryIcon = _icon.get(wxART_CATEGORY);

            std::string fromAccount = t->getFromAccount() ? t->getFromAccount()->getName() : "External";
            std::string toAccount = t->getToAccount() ? t->getToAccount()->getName() : "External";
            std::string category = t->getCategory() ? t->getCategory()->getName() : "None";
            std::string amount = std::to_string(t->getAmount());
            std::string date = t->getDate().FormatISODate().ToStdString();
            

            
            wxStaticBitmap* transactionBitmap = new wxStaticBitmap(dialog, wxID_ANY, transactionIcon);
            wxStaticText* fromText = new wxStaticText(dialog, wxID_ANY, fromAccount);
            wxStaticBitmap* forwardBitmap = new wxStaticBitmap(dialog, wxID_ANY, forwardIcon);
            wxStaticText* toText = new wxStaticText(dialog, wxID_ANY, toAccount);
            wxStaticBitmap* rupeeBitmap = new wxStaticBitmap(dialog, wxID_ANY, rupeeIcon);
            wxStaticText* amountText = new wxStaticText(dialog, wxID_ANY, amount);
            wxStaticBitmap* categoryBitmap = new wxStaticBitmap(dialog, wxID_ANY, categoryIcon);
            wxStaticText* categoryText = new wxStaticText(dialog, wxID_ANY, category);
            wxStaticText* dateText = new wxStaticText(dialog, wxID_ANY, date);
            
            transactionSizer->Add(transactionBitmap, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);

            transactionSizer->Add(dateText, 0, wxALIGN_CENTER_VERTICAL);

            transactionSizer->Add(categoryBitmap, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
            transactionSizer->Add(categoryText, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);

            transactionSizer->Add(fromText, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
            transactionSizer->Add(forwardBitmap, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
            transactionSizer->Add(toText, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
            transactionSizer->Add(rupeeBitmap, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
            transactionSizer->Add(amountText, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
            
            
            
            sizer->Add(transactionSizer, 0, wxEXPAND | wxALL, 5);
        }
        
        dialog->SetSizerAndFit(sizer);
        dialog->ShowModal();
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