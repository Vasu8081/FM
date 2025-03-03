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
        _mainSizer = new wxBoxSizer(wxVERTICAL);
        
        // Accounts Section
        _mainSizer->Add(new wxStaticText(this, wxID_ANY, "Accounts"), 0, wxEXPAND | wxALL, 10);
        _accountScrollWindow = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);
        _accountScrollWindow->SetScrollRate(5, 5);
        _accountSizer = new wxBoxSizer(wxVERTICAL);
        _accountGrid = new wxGridSizer(4, 10, 10);
        _accountSizer->Add(_accountGrid, 1, wxEXPAND | wxALL, 10);
        _accountScrollWindow->SetSizer(_accountSizer);
        _mainSizer->Add(_accountScrollWindow, 1, wxEXPAND | wxALL, 10);
        
        // Categories Section
        _mainSizer->Add(new wxStaticText(this, wxID_ANY, "Categories"), 0, wxEXPAND | wxALL, 10);
        _categoryScrollWindow = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);
        _categoryScrollWindow->SetScrollRate(5, 5);
        _categorySizer = new wxBoxSizer(wxVERTICAL);
        _categoryGrid = new wxGridSizer(4, 10, 10);
        _categorySizer->Add(_categoryGrid, 1, wxEXPAND | wxALL, 10);
        _categoryScrollWindow->SetSizer(_categorySizer);
        _mainSizer->Add(_categoryScrollWindow, 1, wxEXPAND | wxALL, 10);

        updateAccounts();
        updateCategories();

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
    
        _mainSizer->Add(buttonSizer, 0, wxEXPAND | wxALL, 10);
        SetSizerAndFit(_mainSizer);
        Centre();
    }

private:
    wxBoxSizer* _mainSizer;
    wxScrolledWindow* _accountScrollWindow;
    wxScrolledWindow* _categoryScrollWindow;
    wxBoxSizer* _accountSizer;
    wxBoxSizer* _categorySizer;
    wxGridSizer* _accountGrid;
    wxGridSizer* _categoryGrid;
    std::unordered_map<std::string, wxWindow*> _accountViews;
    std::unordered_map<std::string, wxWindow*> _categoryViews;    
    
    model_manager& _models = model_manager::getInstance();

    void updateAccounts() {
        for (auto account : _models.getAccounts()) {
            if (!_accountViews.count(account.first)) {
                auto accountView = model_view_factory::create(_accountScrollWindow, account.second);
                _accountGrid->Add(accountView, 0, wxEXPAND | wxALL, 10);
                _accountViews[account.first] = accountView;
            }
        }
        _accountScrollWindow->FitInside();
        _accountScrollWindow->Layout();
    }

    void updateCategories() {
        for (auto category : _models.getCategories()) {
            if (!_categoryViews.count(category.first)) {
                auto categoryView = model_view_factory::create(_categoryScrollWindow, category.second);
                _categoryGrid->Add(categoryView, 0, wxEXPAND | wxALL, 10);
                _categoryViews[category.first] = categoryView;
            }
        }
        _categoryScrollWindow->FitInside();
        _categoryScrollWindow->Layout();
    }

    void onAddAccountButtonClicked(wxCommandEvent& event) {
        wxDialog* dialog = new wxDialog(this, wxID_ANY, "Select Account Type", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
        wxBoxSizer* dialogSizer = new wxBoxSizer(wxVERTICAL);

        wxArrayString accountTypes;
        accountTypes.Add("Bank Account");
        accountTypes.Add("Borrow Account");
        accountTypes.Add("Give Account");

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
        updateAccounts();
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
        updateCategories();
    }
};

#endif // HOME_HPP