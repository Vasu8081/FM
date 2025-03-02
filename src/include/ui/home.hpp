#ifndef HOME_HPP
#define HOME_HPP

#include <wx/wx.h>
#include <models/models_manager.hpp>

class home : public wxFrame
{
public:
    home() : wxFrame(nullptr, wxID_ANY, "Home", wxDefaultPosition, wxSize(800, 600)) {
#ifdef WIN32
        SetIcon(wxICON(IDI_APPICON));
#else
#endif
        SetMinSize(wxSize(800, 600));
        SetMaxSize(wxSize(800, 600));
        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
        wxStaticText* title = new wxStaticText(this, wxID_ANY, "Home", wxPoint(10, 10), wxSize(100, 20));
        sizer->Add(title, 0, wxEXPAND | wxALL, 10);
        for (auto account: _models.getAccounts()) {
            wxStaticText* account_text = new wxStaticText(this, wxID_ANY, account.second->to_str(), wxPoint(10, 10), wxSize(100, 20));
            sizer->Add(account_text, 0, wxEXPAND | wxALL, 10);
        }

        for (auto transaction: _models.getTransactions()) {
            wxStaticText* transaction_text = new wxStaticText(this, wxID_ANY, transaction->to_str(), wxPoint(10, 10), wxSize(100, 20));
            sizer->Add(transaction_text, 0, wxEXPAND | wxALL, 10);
        }
        SetSizer(sizer);
        Centre();
    }
private:
    model_manager& _models = model_manager::getInstance();
};

#endif // HOME_HPP