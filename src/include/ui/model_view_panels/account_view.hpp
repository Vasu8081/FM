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
    AccountView(wxWindow *parent, std::shared_ptr<Account> model) : ModelView(parent, model)
    {
        _foregroundColour = model->getForegroundColor();
        _backgroundColour = model->getBackgroundColor();
        auto sizer = new wxBoxSizer(wxVERTICAL);
        auto displayFields = model->displayFormFields();
        auto boldFields = model->boldFormFields();
        auto overrideColors = model->overrideFormColors();
        auto hiddenFields = model->hiddenFormFields();
        auto header_row = displayFields[0];
        auto headingSizer = new wxBoxSizer(wxHORIZONTAL);
        auto heading = new wxStaticText(this, wxID_ANY, header_row.second);
        heading->SetForegroundColour(_foregroundColour);
        wxFont font = heading->GetFont();
        font.SetWeight(wxFONTWEIGHT_BOLD);
        font.SetPointSize(font.GetPointSize() + 4);
        heading->SetFont(font);
        headingSizer->Add(heading, 0, wxEXPAND | wxALL, 5);
        
        if(hiddenFields.size() > 0){
            auto eye_icon = _icon.get(wxART_VISIBILITY_OFF, *wxBLACK);
            _eye_button = new wxBitmapButton(this, wxID_ANY, eye_icon, wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, "Toggle Hidden Values");
            _eye_button->Bind(wxEVT_BUTTON, &AccountView::onHiddenValues, this);
            headingSizer->AddStretchSpacer(1);
            headingSizer->Add(_eye_button, 0, wxEXPAND | wxALL, 5);
        }
        sizer->Add(headingSizer, 0, wxEXPAND | wxALL, 5);

        wxStaticBoxSizer *staticBoxSizer = new wxStaticBoxSizer(wxVERTICAL, this);

        for (auto &[key, value] : displayFields)
        {
            if (key == "header")
                continue;
            auto field_sizer = createStaticText(this, key, value);

            if (boldFields.find(key) != boldFields.end())
            {
                wxFont font = _staticTextFields[key]->GetFont();
                font.SetWeight(wxFONTWEIGHT_BOLD);
                _staticTextFields[key]->SetFont(font);
            }
            if (overrideColors.find(key) != overrideColors.end())
            {
                _staticTextFields[key]->SetForegroundColour(overrideColors[key]);
            }

            if(hiddenFields.find(key) != hiddenFields.end()){
                _staticTextFields[key]->SetLabel("********");
            }

            staticBoxSizer->Add(field_sizer, 0, wxEXPAND | wxALL, 1);
        }
        sizer->Add(staticBoxSizer, 0, wxEXPAND | wxALL, 2);

        auto income = _icon.get(wxART_TRENDING_DOWN, wxColour(46, 204, 113));
        auto expense = _icon.get(wxART_TRENDING_UP, *wxRED);

        auto button_sizer = new wxBoxSizer(wxHORIZONTAL);

        wxBitmapButton *incomeButton = new wxBitmapButton(this, wxID_ANY, income, wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, "Income");
        wxBitmapButton *expenseButton = new wxBitmapButton(this, wxID_ANY, expense, wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, "Expense");
        auto view_transactions = new wxButton(this, wxID_ANY, "View Transactions");

        button_sizer->Add(incomeButton, 0, wxALL, 5);
        button_sizer->Add(view_transactions, 1, wxEXPAND | wxALL, 5);
        button_sizer->Add(expenseButton, 0, wxALL, 5);

        Bind(wxEVT_BUTTON, &AccountView::addIncome, this, incomeButton->GetId());
        Bind(wxEVT_BUTTON, &AccountView::addExpense, this, expenseButton->GetId());
        Bind(wxEVT_BUTTON, &AccountView::viewTransactions, this, view_transactions->GetId());

        sizer->Add(button_sizer, 0, wxEXPAND | wxALL, 2);

        SetSizer(sizer);
        Layout();
        Fit();
        SetBackgroundColour(_backgroundColour);
    }

protected:
    std::unordered_map<std::string, wxStaticText *> _staticTextFields;
    wxColour _foregroundColour;
    wxColour _backgroundColour;
    wxBitmapButton *_eye_button;
    bool _show_hidden_values = false;

    wxBoxSizer *createStaticText(wxWindow *parent, const wxString &key, const wxString &value)
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
        box_sizer->Add(staticKey, 0, wxALL, 2);
        box_sizer->Add(staticValue, 0, wxEXPAND | wxALL, 2);

        return box_sizer;
    }

    void addExpense(wxCommandEvent &event){
        wxDialog *transactionDialog = new wxDialog(this, wxID_ANY, "Expense Transaction",  wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE);
        wxBoxSizer *transactionSizer = new wxBoxSizer(wxVERTICAL);
    
        auto transactionForm = new AddModelForm(transactionDialog, "Transaction", _model, true);
        transactionSizer->Add(transactionForm, 1, wxEXPAND | wxALL, 10);
    
        transactionDialog->SetSizer(transactionSizer);
        transactionDialog->Fit();
        transactionDialog->Layout();
        transactionDialog->Centre();
        transactionDialog->ShowModal();
    }
    
    void addIncome(wxCommandEvent &event){
        wxDialog *transactionDialog = new wxDialog(this, wxID_ANY, "Income Transaction", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE);
        wxBoxSizer *transactionSizer = new wxBoxSizer(wxVERTICAL);

        auto transactionForm = new AddModelForm(transactionDialog, "Transaction", _model, false);
        transactionSizer->Add(transactionForm, 1, wxEXPAND | wxALL, 5);

        transactionDialog->SetSizer(transactionSizer);
        transactionDialog->Fit();
        transactionDialog->Layout();
        transactionDialog->Centre();
        transactionDialog->ShowModal();
    }

    void viewTransactions(wxCommandEvent &event)
    {
        auto model = std::dynamic_pointer_cast<Account>(_model);
        auto from_transactions = model->getFromTransactions();
        auto to_transactions = model->getToTransactions();
        auto transactions = from_transactions;
        transactions.insert(transactions.end(), to_transactions.begin(), to_transactions.end());

        std::sort(transactions.begin(), transactions.end(), [](std::shared_ptr<Transaction> a, std::shared_ptr<Transaction> b)
                  { return a->getDate() > b->getDate(); });
        if (transactions.size() <= 0)
        {
            wxMessageBox("No transactions found");
            return;
        }
        auto dialog = new wxDialog(this, wxID_ANY, "Transactions", wxDefaultPosition, wxSize(400, 600), wxDEFAULT_DIALOG_STYLE);
        auto sizer = new wxBoxSizer(wxVERTICAL);

        for (auto &t : transactions)
        {
            wxBoxSizer *transactionSizer = new wxBoxSizer(wxHORIZONTAL);

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

            wxStaticBitmap *transactionBitmap = new wxStaticBitmap(dialog, wxID_ANY, transactionIcon);
            wxStaticText *fromText = new wxStaticText(dialog, wxID_ANY, fromAccount);
            wxStaticBitmap *forwardBitmap = new wxStaticBitmap(dialog, wxID_ANY, forwardIcon);
            wxStaticText *toText = new wxStaticText(dialog, wxID_ANY, toAccount);
            wxStaticBitmap *rupeeBitmap = new wxStaticBitmap(dialog, wxID_ANY, rupeeIcon);
            wxStaticText *amountText = new wxStaticText(dialog, wxID_ANY, amount);
            wxStaticBitmap *categoryBitmap = new wxStaticBitmap(dialog, wxID_ANY, categoryIcon);
            wxStaticText *categoryText = new wxStaticText(dialog, wxID_ANY, category);
            wxStaticText *dateText = new wxStaticText(dialog, wxID_ANY, date);

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
        dialog->Layout();
        dialog->Centre();
        dialog->ShowModal();
    }

    void onHiddenValues(wxCommandEvent &event){
        _show_hidden_values = !_show_hidden_values;
        auto new_icon = _show_hidden_values ? _icon.get(wxART_VISIBILITY, *wxBLACK) : _icon.get(wxART_VISIBILITY_OFF, *wxBLACK);
        _eye_button->SetBitmap(new_icon);
        _eye_button->Refresh();
        if(_show_hidden_values){
            showHiddenValues();
        }
        else{
            hideHiddenValues();
        }
    }

    void showHiddenValues() {
        auto model = std::dynamic_pointer_cast<Account>(_model);
        auto displayFields = model->displayFormFields();
        for (auto &[key, value] : displayFields)
        {
            if (key == "header")
                continue;
            _staticTextFields[key]->SetLabel(value);
        }
    }

    void hideHiddenValues(){
        auto model = std::dynamic_pointer_cast<Account>(_model);
        auto displayFields = model->displayFormFields();
        auto hiddenFields = model->hiddenFormFields();
        for (auto &[key, value] : displayFields)
        {
            if (key == "header")
                continue;
            if(hiddenFields.find(key) != hiddenFields.end()){
                _staticTextFields[key]->SetLabel("********");
            }
            else{
                _staticTextFields[key]->SetLabel(value);
            }
        }
    }

    void update() override
    {
        if(_show_hidden_values){
            showHiddenValues();
        }
        else{
            hideHiddenValues();
        }
    }

};

#endif // BANK_ACCOUNT_VIEW_HPP