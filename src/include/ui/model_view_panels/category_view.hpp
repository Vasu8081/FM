#ifndef CATEGORY_VIEW_HPP
#define CATEGORY_VIEW_HPP

#include <ui/model_view_panels/model_view.hpp>
#include <models/transaction.hpp>
#include <models/account.hpp>
#include <models/category.hpp>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/choice.h>
#include <wx/stattext.h>
#include <wx/statbmp.h>
#include <wx/artprov.h>
#include <wx/dialog.h>

class CategoryView : public ModelView
{
public:
    CategoryView(wxWindow *parent, std::shared_ptr<Category> model) : ModelView(parent, model)
    {
        auto sizer = new wxBoxSizer(wxVERTICAL);
        
        wxStaticBox* staticBox = new wxStaticBox(this, wxID_ANY, "Category Details");
        wxStaticBoxSizer* staticBoxSizer = new wxStaticBoxSizer(staticBox, wxVERTICAL);
        
        _category_name = new wxStaticText(this, wxID_ANY, "Name: " + model->getName());
        _category_description = new wxStaticText(this, wxID_ANY, "Description: " + model->getDescription());
        _category_monthly_budget = new wxStaticText(this, wxID_ANY, "Monthly Budget: " + std::to_string(model->getMonthlyBudget()));
        _category_spent_this_month = new wxStaticText(this, wxID_ANY, "Spent This Month: " + std::to_string(model->getSpentThisMonth()));
        _category_spent_last_month = new wxStaticText(this, wxID_ANY, "Spent Last Month: " + std::to_string(model->getSpentLastMonth()));
        _category_average_three_months = new wxStaticText(this, wxID_ANY, "Average Last 3 Months: " + std::to_string(model->getAverageThreeMonths()));

        staticBoxSizer->Add(_category_name, 0, wxEXPAND | wxALL, 5);
        staticBoxSizer->Add(_category_description, 0, wxEXPAND | wxALL, 5);
        staticBoxSizer->Add(_category_monthly_budget, 0, wxEXPAND | wxALL, 5);
        staticBoxSizer->Add(_category_spent_this_month, 0, wxEXPAND | wxALL, 5);
        staticBoxSizer->Add(_category_spent_last_month, 0, wxEXPAND | wxALL, 5);
        staticBoxSizer->Add(_category_average_three_months, 0, wxEXPAND | wxALL, 5);

        auto view_transactions = new wxButton(this, wxID_ANY, "View Transactions");
        staticBoxSizer->Add(view_transactions, 0, wxEXPAND | wxALL, 5);
        Bind(wxEVT_BUTTON, &CategoryView::viewTransactions, this, view_transactions->GetId());
        
        sizer->Add(staticBoxSizer, 0, wxEXPAND | wxALL, 10);
        
        SetSizer(sizer);
        SetBackgroundColour(wxColour(254, 216, 177));
    }

    void viewTransactions(wxCommandEvent& event){
        auto model = std::dynamic_pointer_cast<Category>(_model);
        auto transactions = model->getTransactions();

        std::sort(transactions.begin(), transactions.end(), [](std::shared_ptr<Transaction> a, std::shared_ptr<Transaction> b){
            return a->getDate() > b->getDate();
        });
        
        auto dialog = new wxDialog(this, wxID_ANY, "Transactions", wxDefaultPosition, wxSize(400, 600), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
        auto sizer = new wxBoxSizer(wxVERTICAL);
        
        for(auto& t : transactions){
            wxBoxSizer* transactionSizer = new wxBoxSizer(wxHORIZONTAL);
            
            wxBitmap forwardIcon = _icon.get(wxART_ARROW_FORWARD);
            wxBitmap rupeeIcon = _icon.get(wxART_CURRENCY_RUPEE);

            std::string fromAccount = t->getFromAccount() ? t->getFromAccount()->getName() : "External";
            std::string toAccount = t->getToAccount() ? t->getToAccount()->getName() : "External";
            std::string amount = std::to_string(t->getAmount());
            std::string date = t->getDate().FormatISODate().ToStdString();
            

            wxStaticText* dateText = new wxStaticText(dialog, wxID_ANY, date);
            wxStaticText* fromText = new wxStaticText(dialog, wxID_ANY, fromAccount);
            wxStaticBitmap* forwardBitmap = new wxStaticBitmap(dialog, wxID_ANY, forwardIcon);
            wxStaticText* toText = new wxStaticText(dialog, wxID_ANY, toAccount);
            wxStaticBitmap* rupeeBitmap = new wxStaticBitmap(dialog, wxID_ANY, rupeeIcon);
            wxStaticText* amountText = new wxStaticText(dialog, wxID_ANY, amount);

            transactionSizer->Add(dateText, 0, wxALIGN_CENTER_VERTICAL);
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
        auto model = std::dynamic_pointer_cast<Category>(_model);
        _category_name->SetLabel("Name: " + model->getName());
        _category_description->SetLabel("Description: " + model->getDescription());
        _category_monthly_budget->SetLabel("Monthly Budget: " + std::to_string(model->getMonthlyBudget()));
        _category_spent_this_month->SetLabel("Spent This Month: " + std::to_string(model->getSpentThisMonth()));
        _category_spent_last_month->SetLabel("Spent Last Month: " + std::to_string(model->getSpentLastMonth()));
        _category_average_three_months->SetLabel("Average Last 3 Months: " + std::to_string(model->getAverageThreeMonths()));
    }
    
private:
    wxStaticText* _category_name;
    wxStaticText* _category_description;
    wxStaticText* _category_monthly_budget;
    wxStaticText* _category_spent_this_month;
    wxStaticText* _category_spent_last_month;
    wxStaticText* _category_average_three_months;
};

#endif // BANK_ACCOUNT_VIEW_HPP