#ifndef RD_ACCOUNT_VIEW_HPP
#define RD_ACCOUNT_VIEW_HPP

#include <ui/model_view_panels/account_view.hpp>
#include <models/rd_account.hpp>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/choice.h>
#include <wx/stattext.h>
#include <wx/statbmp.h>

class RdAccountView : public AccountView
{
public:
    RdAccountView(wxWindow* parent, std::shared_ptr<RdAccount> model) : AccountView(parent, model) {
        auto sizer = new wxBoxSizer(wxVERTICAL);
        
        wxStaticBox* staticBox = new wxStaticBox(this, wxID_ANY, "Rd Details");
        wxStaticBoxSizer* staticBoxSizer = new wxStaticBoxSizer(staticBox, wxVERTICAL);

        _rd_name = new wxStaticText(this, wxID_ANY, "RD Name: " + model->getName());
        _monthly_payment = new wxStaticText(this, wxID_ANY, "Monthly Payment: " + std::to_string(model->getMonthlyPayment()));
        _paid_amount = new wxStaticText(this, wxID_ANY, "Paid Amount: " + std::to_string(model->getPaidAmount()));
        _maturity_amount = new wxStaticText(this, wxID_ANY, "Maturity Amount: " + std::to_string(model->getMaturityAmount()));
        _start_date = new wxStaticText(this, wxID_ANY, "Start Date: " + model->getStartDate().FormatISODate());
        _maturity_date = new wxStaticText(this, wxID_ANY, "Maturity Date: " + model->getMaturityDate().FormatISODate());
        _monthly_payment_date = new wxStaticText(this, wxID_ANY, "Monthly Payment Date: " + std::to_string(model->getMonthlyPaymentDate().GetDay()) + "th of every month");
        _monthly_payment->SetFont(_monthly_payment->GetFont().Bold());
        _paid_amount->SetFont(_paid_amount->GetFont().Bold());
        _maturity_amount->SetFont(_maturity_amount->GetFont().Bold());

        staticBoxSizer->Add(_rd_name, 0, wxEXPAND | wxALL, 5);
        staticBoxSizer->Add(_monthly_payment, 0, wxEXPAND | wxALL, 5);
        staticBoxSizer->Add(_paid_amount, 0, wxEXPAND | wxALL, 5);
        staticBoxSizer->Add(_maturity_amount, 0, wxEXPAND | wxALL, 5);
        staticBoxSizer->Add(_start_date, 0, wxEXPAND | wxALL, 5);
        staticBoxSizer->Add(_maturity_date, 0, wxEXPAND | wxALL, 5);
        staticBoxSizer->Add(_monthly_payment_date, 0, wxEXPAND | wxALL, 5);

        auto view_transactions = new wxButton(this, wxID_ANY, "View Transactions");
        staticBoxSizer->Add(view_transactions, 0, wxEXPAND | wxALL, 5);
        Bind(wxEVT_BUTTON, &RdAccountView::viewTransactions, this, view_transactions->GetId());

        sizer->Add(staticBoxSizer, 0, wxEXPAND | wxALL, 10);

        SetSizer(sizer);
        SetBackgroundColour(wxColour(240, 248, 255));
    }

    void update() override {
        auto model = std::dynamic_pointer_cast<RdAccount>(_model);
        _paid_amount->SetLabelText(std::to_string(model->getPaidAmount()));
    }
private:
    wxStaticText* _rd_name;
    wxStaticText* _monthly_payment;
    wxStaticText* _paid_amount;
    wxStaticText* _maturity_amount;
    wxStaticText* _start_date;
    wxStaticText* _maturity_date;
    wxStaticText* _monthly_payment_date;
};

#endif // RD_ACCOUNT_VIEW_HPP