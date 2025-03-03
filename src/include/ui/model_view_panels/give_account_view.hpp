#ifndef GIVE_ACCOUNT_VIEW_HPP
#define GIVE_ACCOUNT_VIEW_HPP

#include <ui/model_view_panels/account_view.hpp>
#include <models/give_account.hpp>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/choice.h>
#include <wx/stattext.h>
#include <wx/statbmp.h>

class GiveAccountView : public AccountView
{
public:
GiveAccountView(wxWindow *parent, std::shared_ptr<give_account> model) : AccountView(parent, model)
{
    auto sizer = new wxBoxSizer(wxVERTICAL);
    
    wxStaticBox* staticBox = new wxStaticBox(this, wxID_ANY, "Borrow Details");
    wxStaticBoxSizer* staticBoxSizer = new wxStaticBoxSizer(staticBox, wxVERTICAL);
    
    _given_to = new wxStaticText(this, wxID_ANY, "Given To: " + model->getGivenTo());
    _givenAmount = new wxStaticText(this, wxID_ANY, "Remaining Given Amount: " + std::to_string(model->getGivenAmount()));
    _givenDate = new wxStaticText(this, wxID_ANY, "Given Date: " + model->getGivenDate().FormatISODate());
    _dueDate = new wxStaticText(this, wxID_ANY, "Due Date: " + model->getDueDate().FormatISODate());
    _given_to->SetFont(_given_to->GetFont().Bold());
    _givenAmount->SetFont(_givenAmount->GetFont().Bold());
    
    staticBoxSizer->Add(_given_to, 0, wxEXPAND | wxALL, 5);
    staticBoxSizer->Add(_givenAmount, 0, wxEXPAND | wxALL, 5);
    staticBoxSizer->Add(_givenDate, 0, wxEXPAND | wxALL, 5);
    staticBoxSizer->Add(_dueDate, 0, wxEXPAND | wxALL, 5);

    
    auto view_transactions = new wxButton(this, wxID_ANY, "View Transactions");
    staticBoxSizer->Add(view_transactions, 0, wxEXPAND | wxALL, 5);
    Bind(wxEVT_BUTTON, &GiveAccountView::viewTransactions, this, view_transactions->GetId());
    
    sizer->Add(staticBoxSizer, 0, wxEXPAND | wxALL, 10);
    
    SetSizer(sizer);
    SetBackgroundColour(wxColour(206, 250, 208));
}

void update() override
{
    auto model = std::dynamic_pointer_cast<give_account>(_model);
    _givenAmount->SetLabelText("Remaining Given Amount: " + std::to_string(model->getGivenAmount()));
}

private:
    wxStaticText* _given_to;
    wxStaticText* _givenAmount;
    wxStaticText* _givenDate;
    wxStaticText* _dueDate;
};

#endif // BORROW_ACCOUNT_VIEW_HPP