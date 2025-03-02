#ifndef HOME_HPP
#define HOME_HPP

#include <wx/wx.h>
#include <wx/notebook.h>
#include <models/models_manager.hpp>
#include <ui/add_model.hpp>

class home : public wxFrame
{
public:
    home() : wxFrame(nullptr, wxID_ANY, "Home", wxDefaultPosition, wxSize(800, 600)) {
#ifdef WIN32
        SetIcon(wxICON(IDI_APPICON));
#else
#endif
        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

        // Add Account Button
        wxButton* addAccountButton = new wxButton(this, wxID_ANY, "Add Account");
        sizer->Add(addAccountButton, 0, wxEXPAND | wxALL, 10);

        addAccountButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event) {
            wxDialog* dialog = new wxDialog(this, wxID_ANY, "Select Account Type", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
            wxBoxSizer* dialogSizer = new wxBoxSizer(wxVERTICAL);

            wxArrayString accountTypes;
            accountTypes.Add("Bank Account");
            accountTypes.Add("Loan Account");
            accountTypes.Add("Investment Account");

            wxChoice* choice = new wxChoice(dialog, wxID_ANY, wxDefaultPosition, wxDefaultSize, accountTypes);
            choice->SetSelection(0);
            dialogSizer->Add(choice, 0, wxEXPAND | wxALL, 10);

            wxButton* nextButton = new wxButton(dialog, wxID_OK, "Next");
            dialogSizer->Add(nextButton, 0, wxALIGN_CENTER | wxALL, 10);

            dialog->SetSizer(dialogSizer);
            dialog->Centre();

            if (dialog->ShowModal() == wxID_OK) {
                wxString selectedType = choice->GetStringSelection();
                wxDialog* accountDialog = new wxDialog(this, wxID_ANY, "Add Account", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
                wxBoxSizer* accountSizer = new wxBoxSizer(wxVERTICAL);

                auto accountForm = new AddModelForm(accountDialog, selectedType.ToStdString());
                accountSizer->Add(accountForm, 1, wxEXPAND | wxALL, 5);

                accountDialog->SetSizer(accountSizer);
                accountDialog->Centre();
                accountDialog->ShowModal();
            }
        });

        // Add Transaction Button
        wxButton* addTransactionButton = new wxButton(this, wxID_ANY, "Add Transaction");
        sizer->Add(addTransactionButton, 0, wxEXPAND | wxALL, 10);

        addTransactionButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event) {
            wxDialog* transactionDialog = new wxDialog(this, wxID_ANY, "Add Transaction", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
            wxBoxSizer* transactionSizer = new wxBoxSizer(wxVERTICAL);

            auto transactionForm = new AddModelForm(transactionDialog, "Transaction");
            transactionSizer->Add(transactionForm, 1, wxEXPAND | wxALL, 5);

            transactionDialog->SetSizer(transactionSizer);
            transactionDialog->Centre();
            transactionDialog->ShowModal();
        });

        // Account Listings
        wxStaticText* accountTitle = new wxStaticText(this, wxID_ANY, "Accounts");
        sizer->Add(accountTitle, 0, wxEXPAND | wxALL, 10);

        for (auto account : _models.getAccounts()) {
            wxStaticText* accountText = new wxStaticText(this, wxID_ANY, account.second->toStr());
            sizer->Add(accountText, 0, wxEXPAND | wxALL, 10);
        }

        // Transaction Listings
        wxStaticText* transactionTitle = new wxStaticText(this, wxID_ANY, "Transactions");
        sizer->Add(transactionTitle, 0, wxEXPAND | wxALL, 10);

        for (auto transaction : _models.getTransactions()) {
            wxStaticText* transactionText = new wxStaticText(this, wxID_ANY, transaction->toStr());
            sizer->Add(transactionText, 0, wxEXPAND | wxALL, 10);
        }

        SetSizer(sizer);
        Centre();
    }

private:
    model_manager& _models = model_manager::getInstance();
};

#endif // HOME_HPP
