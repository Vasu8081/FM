#ifndef HOME_HPP
#define HOME_HPP

#include <wx/wx.h>
#include <wx/notebook.h>
#include <models/models_manager.hpp>
#include <ui/account/add_account.hpp>

class home : public wxFrame
{
public:
    home() : wxFrame(nullptr, wxID_ANY, "Home", wxDefaultPosition, wxSize(800, 600)) {
#ifdef WIN32
        SetIcon(wxICON(IDI_APPICON));
#else
#endif
        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

        notebook = new wxNotebook(this, wxID_ANY);
        addAccountPanel = new AddAccountPanel(notebook);
        notebook->AddPage(addAccountPanel, "Add Account");
        
        sizer->Add(notebook, 1, wxEXPAND | wxALL, 5);

        wxStaticText* title = new wxStaticText(this, wxID_ANY, "Accounts", wxPoint(10, 10), wxSize(100, 20));
        sizer->Add(title, 0, wxEXPAND | wxALL, 10);
        for (auto account: _models.getAccounts()) {
            wxStaticText* account_text = new wxStaticText(this, wxID_ANY, account.second->to_str(), wxPoint(10, 10), wxSize(100, 20));
            sizer->Add(account_text, 0, wxEXPAND | wxALL, 10);
        }

        wxStaticText* title2 = new wxStaticText(this, wxID_ANY, "Transactions", wxPoint(10, 10), wxSize(100, 20));
        sizer->Add(title2, 0, wxEXPAND | wxALL, 10);
        for (auto transaction: _models.getTransactions()) {
            wxStaticText* transaction_text = new wxStaticText(this, wxID_ANY, transaction->to_str(), wxPoint(10, 10), wxSize(100, 20));
            sizer->Add(transaction_text, 0, wxEXPAND | wxALL, 10);
        }
        
        SetSizer(sizer);
        Centre();
    }
private:
    model_manager& _models = model_manager::getInstance();
    AddAccountPanel* addAccountPanel;
    wxNotebook* notebook;
};

#endif // HOME_HPP