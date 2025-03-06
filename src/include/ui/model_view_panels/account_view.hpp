#ifndef ACCOUNT_VIEW_HPP
#define ACCOUNT_VIEW_HPP

#include <ui/model_view_panels/model_view.hpp>
#include <models/account.hpp>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/choice.h>
#include <wx/stattext.h>
#include <wx/statbmp.h>
#include <wx/artprov.h>
#include <wx/dialog.h>

class AccountView : public ModelView
{
public:
    AccountView(wxWindow *parent, std::shared_ptr<Account> model) : ModelView(parent, model) {}

protected:

    void viewTransactions(wxCommandEvent& event){
        auto model = std::dynamic_pointer_cast<Account>(_model);
        auto from_transactions = model->getFromTransactions();
        auto to_transactions = model->getToTransactions();
        auto transactions = from_transactions;
        transactions.insert(transactions.end(), to_transactions.begin(), to_transactions.end());
        
        std::sort(transactions.begin(), transactions.end(), [](std::shared_ptr<Transaction> a, std::shared_ptr<Transaction> b){
            return a->getDate() > b->getDate();
        });
        
        auto dialog = new wxDialog(this, wxID_ANY, "Transactions", wxDefaultPosition, wxSize(400, 600), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
        auto sizer = new wxBoxSizer(wxVERTICAL);
        
        for(auto& t : transactions){
            wxBoxSizer* transactionSizer = new wxBoxSizer(wxHORIZONTAL);

            bool isOutgoing = (t->getFromAccount() && t->getFromAccount()->getID() == model->getID());
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

    virtual void update() override = 0;
};

#endif // BANK_ACCOUNT_VIEW_HPP