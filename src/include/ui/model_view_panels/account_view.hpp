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
    AccountView(wxWindow *parent, std::shared_ptr<Account> model) : ModelView(parent, model) {
        _foregroundColour = model->getForegroundColor();
        _backgroundColour = model->getBackgroundColor();
        auto sizer = new wxBoxSizer(wxVERTICAL);
        auto displayFields = model->displayFormFields();
        auto boldFields = model->boldFormFields();
        auto overrideColors = model->overrideFormColors();
        auto staticBox = new wxStaticBox(this, wxID_ANY, displayFields["header"]);
        if (boldFields.find("header") != boldFields.end()) {
            wxFont font = staticBox->GetFont();
            font.SetWeight(wxFONTWEIGHT_BOLD);
            staticBox->SetFont(font);
        }
        staticBox->SetForegroundColour(_foregroundColour);
        wxStaticBoxSizer* staticBoxSizer = new wxStaticBoxSizer(staticBox, wxVERTICAL);
        
        
        for (auto& [key, value] : displayFields)
        {
            if(key == "header") continue;
            auto field_sizer = createStaticText(this, key , value);

            if (boldFields.find(key) != boldFields.end()) {
                wxFont font = _staticTextFields[key]->GetFont();
                font.SetWeight(wxFONTWEIGHT_BOLD);
                _staticTextFields[key]->SetFont(font);
            }
            if(overrideColors.find(key)!= overrideColors.end()){
                _staticTextFields[key]->SetForegroundColour(overrideColors[key]);
            }
            
            staticBoxSizer->Add(field_sizer, 0, wxEXPAND | wxALL, 5);
        }
        
        auto view_transactions = new wxButton(this, wxID_ANY, "View Transactions");
        staticBoxSizer->Add(view_transactions, 0, wxEXPAND | wxALL, 5);
        Bind(wxEVT_BUTTON, &AccountView::viewTransactions, this, view_transactions->GetId());
        
        sizer->Add(staticBoxSizer, 0, wxEXPAND | wxALL, 10);
        
        SetSizer(sizer);
        SetBackgroundColour(_backgroundColour);
    }

protected:

    std::unordered_map<std::string, wxStaticText*> _staticTextFields;

    wxBoxSizer* createStaticText(wxWindow* parent, const wxString& key, const wxString& value)
    {
        auto box_sizer = new wxBoxSizer(wxHORIZONTAL);
        auto staticKey = new wxStaticText(parent, wxID_ANY, key + " : ");
        staticKey->SetForegroundColour(_foregroundColour);
        wxFont font = staticKey->GetFont();
        font.SetWeight(wxFONTWEIGHT_BOLD);
        staticKey->SetFont(font);

        auto staticValue = new wxStaticText(parent, wxID_ANY, value);
        staticValue->SetForegroundColour(_foregroundColour);

        _staticTextFields[key.ToStdString()] = staticValue;
        box_sizer->Add(staticKey, 0, wxALL, 10);
        box_sizer->Add(staticValue, 0, wxEXPAND | wxALL, 10);

        return box_sizer;
    }

    wxColour _foregroundColour;
    wxColour _backgroundColour;

    void viewTransactions(wxCommandEvent& event){
        auto model = std::dynamic_pointer_cast<Account>(_model);
        auto from_transactions = model->getFromTransactions();
        auto to_transactions = model->getToTransactions();
        auto transactions = from_transactions;
        transactions.insert(transactions.end(), to_transactions.begin(), to_transactions.end());
        
        std::sort(transactions.begin(), transactions.end(), [](std::shared_ptr<Transaction> a, std::shared_ptr<Transaction> b){
            return a->getDate() > b->getDate();
        });
        if(transactions.size() <= 0){
            wxMessageBox("No transactions found");
            return;
        }
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
            std::string amount = Formatter::Amount(t->getAmount());
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
        wxMessageBox("Product Updated");
        auto model = std::dynamic_pointer_cast<Account>(_model);
        auto displayFields = model->displayFormFields();
        for (auto& [key, value] : displayFields)
        {
            if(key == "header") continue;
            _staticTextFields[key]->SetLabel(value);
        }
    }
};

#endif // BANK_ACCOUNT_VIEW_HPP