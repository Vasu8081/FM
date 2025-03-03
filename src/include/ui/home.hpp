#ifndef HOME_HPP
#define HOME_HPP

#include <wx/wx.h>
#include <wx/notebook.h>
#include <models/models_manager.hpp>
#include <ui/add_model.hpp>
#include <ui/model_view_panels/model_view_factory.hpp>

class home : public wxFrame
{
public:
home() : wxFrame(nullptr, wxID_ANY, "Home", wxDefaultPosition, wxSize(800, 600)) {
    #ifdef WIN32
        SetIcon(wxICON(IDI_APPICON));
    #else
    #endif
        wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    
        // Accounts Section
        mainSizer->Add(new wxStaticText(this, wxID_ANY, "Accounts"), 0, wxEXPAND | wxALL, 10);
        wxScrolledWindow* accountScrollWindow = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 300), wxVSCROLL);
        accountScrollWindow->SetScrollRate(5, 5);
        wxBoxSizer* accountSizer = new wxBoxSizer(wxVERTICAL);
        wxGridSizer* accountGrid = new wxGridSizer(4, 10, 10);
        
        int accountCount = 0;
        for (auto account : _models.getAccounts()) {
            auto accountView = model_view_factory::create(accountScrollWindow, account.second);
            accountGrid->Add(accountView, 0, wxEXPAND | wxALL, 10);
            accountCount++;
        }
        
        accountScrollWindow->SetSizer(accountGrid);
        accountScrollWindow->FitInside();
        accountSizer->Add(accountScrollWindow, 1, wxEXPAND | wxALL, 10);
        mainSizer->Add(accountSizer, 0, wxEXPAND | wxALL, 10);
    
        // Categories Section
        mainSizer->Add(new wxStaticText(this, wxID_ANY, "Categories"), 0, wxEXPAND | wxALL, 10);
        wxScrolledWindow* categoryScrollWindow = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 250), wxVSCROLL);
        categoryScrollWindow->SetScrollRate(5, 5);
        wxBoxSizer* categorySizer = new wxBoxSizer(wxVERTICAL);
        wxGridSizer* categoryGrid = new wxGridSizer(4, 10, 10);
        
        int categoryCount = 0;
        for (auto category : _models.getCategories()) {
            auto categoryView = model_view_factory::create(categoryScrollWindow, category.second);
            categoryGrid->Add(categoryView, 0, wxEXPAND | wxALL, 10);
            categoryCount++;
        }
        
        categoryScrollWindow->SetSizer(categoryGrid);
        categoryScrollWindow->FitInside();
        categorySizer->Add(categoryScrollWindow, 1, wxEXPAND | wxALL, 10);
        mainSizer->Add(categorySizer, 0, wxEXPAND | wxALL, 10);
    
        // Buttons (Always Visible)
        wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
        wxButton* addAccountButton = new wxButton(this, wxID_ANY, "Add Account");
        buttonSizer->Add(addAccountButton, 1, wxEXPAND | wxALL, 10);
        addAccountButton->Bind(wxEVT_BUTTON, &home::onAddAccountButtonClicked, this);
        
        wxButton* addTransactionButton = new wxButton(this, wxID_ANY, "Add Transaction");
        buttonSizer->Add(addTransactionButton, 1, wxEXPAND | wxALL, 10);
        addTransactionButton->Bind(wxEVT_BUTTON, &home::onAddTransactionButtonClicked, this);
        
        wxButton* addCategoryButton = new wxButton(this, wxID_ANY, "Add Category");
        buttonSizer->Add(addCategoryButton, 1, wxEXPAND | wxALL, 10);
        addCategoryButton->Bind(wxEVT_BUTTON, &home::onAddCategoryButtonClicked, this);
    
        mainSizer->Add(buttonSizer, 0, wxEXPAND | wxALL, 10);
    
        SetSizer(mainSizer);
        Centre();
    }

private:
    model_manager& _models = model_manager::getInstance();

    void onAddAccountButtonClicked(wxCommandEvent& event) {
        wxDialog* dialog = new wxDialog(this, wxID_ANY, "Select Account Type", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
        wxBoxSizer* dialogSizer = new wxBoxSizer(wxVERTICAL);

        wxArrayString accountTypes;
        accountTypes.Add("Bank Account");

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
    }

    void onAddTransactionButtonClicked(wxCommandEvent& event) {
        wxDialog* transactionDialog = new wxDialog(this, wxID_ANY, "Add Transaction", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
        wxBoxSizer* transactionSizer = new wxBoxSizer(wxVERTICAL);

        auto transactionForm = new AddModelForm(transactionDialog, "Transaction");
        transactionSizer->Add(transactionForm, 1, wxEXPAND | wxALL, 5);

        transactionDialog->SetSizer(transactionSizer);
        transactionDialog->Centre();
        transactionDialog->ShowModal();
    }

    void onAddCategoryButtonClicked(wxCommandEvent& event) {
        wxDialog* categoryDialog = new wxDialog(this, wxID_ANY, "Add Category", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
        wxBoxSizer* categorySizer = new wxBoxSizer(wxVERTICAL);

        auto categoryForm = new AddModelForm(categoryDialog, "Category");
        categorySizer->Add(categoryForm, 1, wxEXPAND | wxALL, 5);

        categoryDialog->SetSizer(categorySizer);
        categoryDialog->Centre();
        categoryDialog->ShowModal();
    }

};

#endif // HOME_HPP
