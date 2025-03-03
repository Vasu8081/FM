#ifndef BORROW_ACCOUNT_VIEW_HPP
#define BORROW_ACCOUNT_VIEW_HPP

#include <ui/model_view_panels/account_view.hpp>
#include <models/borrow_account.hpp>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/choice.h>
#include <wx/stattext.h>
#include <wx/statbmp.h>

class BorrowAccountView : public AccountView
{
public:
BorrowAccountView(wxWindow *parent, std::shared_ptr<borrow_account> model) : AccountView(parent, model)
{
    auto sizer = new wxBoxSizer(wxVERTICAL);
    
    wxStaticBox* staticBox = new wxStaticBox(this, wxID_ANY, "Borrow Details");
    wxStaticBoxSizer* staticBoxSizer = new wxStaticBoxSizer(staticBox, wxVERTICAL);
    
    _borrower = new wxStaticText(this, wxID_ANY, "Borrower: " + model->getBorrower());
    _borrowedAmount = new wxStaticText(this, wxID_ANY, "Remaining Borrowed Amount: " + std::to_string(model->getBorrowedAmount()));
    _borrowedDate = new wxStaticText(this, wxID_ANY, "Borrowed Date: " + model->getBorrowedDate().FormatISODate());
    _dueDate = new wxStaticText(this, wxID_ANY, "Due Date: " + model->getDueDate().FormatISODate());
    _borrower->SetFont(_borrower->GetFont().Bold());
    _borrowedAmount->SetFont(_borrowedAmount->GetFont().Bold());
    
    staticBoxSizer->Add(_borrower, 0, wxEXPAND | wxALL, 5);
    staticBoxSizer->Add(_borrowedAmount, 0, wxEXPAND | wxALL, 5);
    staticBoxSizer->Add(_borrowedDate, 0, wxEXPAND | wxALL, 5);
    staticBoxSizer->Add(_dueDate, 0, wxEXPAND | wxALL, 5);

    
    auto view_transactions = new wxButton(this, wxID_ANY, "View Transactions");
    staticBoxSizer->Add(view_transactions, 0, wxEXPAND | wxALL, 5);
    Bind(wxEVT_BUTTON, &BorrowAccountView::viewTransactions, this, view_transactions->GetId());
    
    sizer->Add(staticBoxSizer, 0, wxEXPAND | wxALL, 10);
    
    SetSizer(sizer);
    SetBackgroundColour(wxColour(254, 216, 177));
}

void update() override
{
    auto model = std::dynamic_pointer_cast<borrow_account>(_model);
    _borrowedAmount->SetLabelText("Remaining Borrowed Amount: " + std::to_string(model->getBorrowedAmount()));
}

private:
    wxStaticText* _borrower;
    wxStaticText* _upiId;
    wxStaticText* _borrowedAmount;
    wxStaticText* _borrowedDate;
    wxStaticText* _dueDate;
};

#endif // BORROW_ACCOUNT_VIEW_HPP