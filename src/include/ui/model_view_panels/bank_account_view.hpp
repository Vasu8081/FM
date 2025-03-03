#ifndef BANK_ACCOUNT_VIEW_HPP
#define BANK_ACCOUNT_VIEW_HPP

#include <ui/model_view_panels/model_view.hpp>
#include <models/bank_account.hpp>

class BankAccountView : public ModelView
{
public:
    BankAccountView(wxWindow *parent, std::shared_ptr<bank_account> model) : ModelView(parent, model)
    {
        auto sizer = new wxBoxSizer(wxVERTICAL);
        _account_name = new wxStaticText(this, wxID_ANY, model->getBankName());
        _account_number = new wxStaticText(this, wxID_ANY, model->getAccountNumber());
        _account_balance = new wxStaticText(this, wxID_ANY, std::to_string(model->getBalance()));
        _ifsc = new wxStaticText(this, wxID_ANY, model->getIfsc());
        _branch = new wxStaticText(this, wxID_ANY, model->getBranch());
        sizer->Add(_account_name, 0, wxEXPAND | wxALL, 5);
        sizer->Add(_account_number, 0, wxEXPAND | wxALL, 5);
        sizer->Add(_account_balance, 0, wxEXPAND | wxALL, 5);
        sizer->Add(_ifsc, 0, wxEXPAND | wxALL, 5);
        sizer->Add(_branch, 0, wxEXPAND | wxALL, 5);
        auto view_transactions = new wxButton(this, wxID_ANY, "View Transactions");
        sizer->Add(view_transactions, 0, wxEXPAND | wxALL, 5);
        Bind(wxEVT_BUTTON, &BankAccountView::viewTransactions, this, view_transactions->GetId());
        SetSizer(sizer);
    }

    void viewTransactions(wxCommandEvent& event){
        auto model = std::dynamic_pointer_cast<bank_account>(_model);
        auto from_transactions = model->getFromTransactions();
        auto to_transactions = model->getToTransactions();
        auto transactions = from_transactions;
        transactions.insert(transactions.end(), to_transactions.begin(), to_transactions.end());
        sort(transactions.begin(), transactions.end(), [](std::shared_ptr<transaction> a, std::shared_ptr<transaction> b){
            return a->getDate() > b->getDate();
        });
        auto dialog = new wxDialog(this, wxID_ANY, "Transactions");
        auto sizer = new wxBoxSizer(wxVERTICAL);
        for(auto t : transactions){
            auto text = new wxStaticText(dialog, wxID_ANY, t->toStr());
            sizer->Add(text, 0, wxEXPAND | wxALL, 5);
        }
        dialog->SetSizer(sizer);
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