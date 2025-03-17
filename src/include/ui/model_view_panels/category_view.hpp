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
        _foregroundColour = model->getForegroundColor();
        _backgroundColour = model->getBackgroundColor();
        auto sizer = new wxBoxSizer(wxVERTICAL);
        auto displayFields = model->displayFormFields();
        auto boldFields = model->boldFormFields();
        auto overrideColors = model->overrideFormColors();
        auto header_row = displayFields[0];
        auto headingSizer = new wxBoxSizer(wxHORIZONTAL);
        auto heading = new wxStaticText(this, wxID_ANY, header_row.second);
        heading->SetForegroundColour(_foregroundColour);
        wxFont font = heading->GetFont();
        font.SetWeight(wxFONTWEIGHT_BOLD);
        font.SetPointSize(font.GetPointSize() + 4);
        heading->SetFont(font);
        headingSizer->Add(heading, 0, wxEXPAND | wxALL, 5);
        auto edit_icon = _icon.get(wxART_EDIT, *wxBLACK);
        auto edit_button = new wxBitmapButton(this, wxID_ANY, edit_icon, wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, "Edit Model");
        edit_button->Bind(wxEVT_BUTTON, &CategoryView::editModel, this);
        headingSizer->AddStretchSpacer(1);
        headingSizer->Add(edit_button, 0, wxEXPAND | wxALL, 5);

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

            staticBoxSizer->Add(field_sizer, 0, wxEXPAND | wxALL, 1);
        }
        sizer->Add(staticBoxSizer, 0, wxEXPAND | wxALL, 2);
        auto button_sizer = new wxBoxSizer(wxHORIZONTAL);
        auto view_transactions = new wxButton(this, wxID_ANY, "View Transactions");
        button_sizer->Add(view_transactions, 1, wxEXPAND | wxALL, 5);

        Bind(wxEVT_BUTTON, &CategoryView::viewTransactions, this, view_transactions->GetId());

        sizer->Add(button_sizer, 0, wxEXPAND | wxALL, 2);

        SetSizer(sizer);
        Layout();
        Fit();
        SetBackgroundColour(_backgroundColour);
    }

    void viewTransactions(wxCommandEvent &event)
    {
        auto model = std::dynamic_pointer_cast<Category>(_model);
        auto transactions = model->getTransactions();

        std::sort(transactions.begin(), transactions.end(), [](std::shared_ptr<Transaction> a, std::shared_ptr<Transaction> b)
                  { return a->getDate() > b->getDate(); });

        auto dialog = new wxDialog(this, wxID_ANY, "Transactions", wxDefaultPosition, wxSize(400, 600), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
        auto sizer = new wxBoxSizer(wxVERTICAL);

        if (transactions.size() <= 0)
        {
            wxMessageBox("No transactions found");
            return;
        }

        for (auto &t : transactions)
        {
            wxBoxSizer *transactionSizer = new wxBoxSizer(wxHORIZONTAL);

            wxBitmap forwardIcon = _icon.get(wxART_ARROW_FORWARD);
            wxBitmap rupeeIcon = _icon.get(wxART_CURRENCY_RUPEE);

            std::string fromAccount = t->getFromAccount() ? t->getFromAccount()->getName() : "External";
            std::string toAccount = t->getToAccount() ? t->getToAccount()->getName() : "External";
            std::string amount = std::to_string(t->getAmount());
            std::string date = t->getDate().FormatISODate().ToStdString();

            wxStaticText *dateText = new wxStaticText(dialog, wxID_ANY, date);
            wxStaticText *fromText = new wxStaticText(dialog, wxID_ANY, fromAccount);
            wxStaticBitmap *forwardBitmap = new wxStaticBitmap(dialog, wxID_ANY, forwardIcon);
            wxStaticText *toText = new wxStaticText(dialog, wxID_ANY, toAccount);
            wxStaticBitmap *rupeeBitmap = new wxStaticBitmap(dialog, wxID_ANY, rupeeIcon);
            wxStaticText *amountText = new wxStaticText(dialog, wxID_ANY, amount);

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

    void editModel(wxCommandEvent &event){
        wxDialog *modelDialog = new wxDialog(this, wxID_ANY, "Edit Model", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE);
        wxBoxSizer *modelSizer = new wxBoxSizer(wxVERTICAL);

        auto modelForm = new EditModelForm(modelDialog, _model->getType(), _model);
        modelSizer->Add(modelForm, 1, wxEXPAND | wxALL, 5);

        modelDialog->SetSizer(modelSizer);
        modelDialog->Fit();
        modelDialog->Layout();
        modelDialog->Centre();
        modelDialog->ShowModal();
    }

    void update() override
    {
        auto model = std::dynamic_pointer_cast<Category>(_model);
        auto displayFields = model->displayFormFields();
        for (auto &[key, value] : displayFields)
        {
            if (key == "header")
                continue;
            _staticTextFields[key]->SetLabel(value);
        }
    }
};

#endif // BANK_ACCOUNT_VIEW_HPP