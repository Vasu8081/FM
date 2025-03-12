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
    home() : wxFrame(nullptr, wxID_ANY, "Home", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE | wxMAXIMIZE)
    {
#ifdef WIN32
        SetIcon(wxICON(IDI_APPICON));
#else
#endif
        _mainSizer = new wxBoxSizer(wxVERTICAL);

        _notebook = new wxNotebook(this, wxID_ANY);

        // Accounts Tab
        wxPanel *accountPanel = new wxPanel(_notebook);
        wxBoxSizer *accountSizer = new wxBoxSizer(wxVERTICAL);
        accountSizer->Add(new wxStaticText(accountPanel, wxID_ANY, "Here you can manage all your financial accounts, segregated by type."), 0, wxEXPAND | wxALL, 10);

        _accountNotebook = new wxNotebook(accountPanel, wxID_ANY);
        accountSizer->Add(_accountNotebook, 1, wxEXPAND | wxALL, 10);

        accountPanel->SetSizer(accountSizer);
        _notebook->AddPage(accountPanel, "Accounts");

        // Categories Tab
        wxPanel *categoryPanel = new wxPanel(_notebook);
        wxBoxSizer *categorySizer = new wxBoxSizer(wxVERTICAL);
        _categoryScrollWindow = new wxScrolledWindow(categoryPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL | wxHSCROLL);
        _categoryScrollWindow->SetScrollRate(5, 5);
        _categorySizer = new wxBoxSizer(wxVERTICAL);
        _categoryScrollWindow->SetSizer(_categorySizer);
        categorySizer->Add(_categoryScrollWindow, 1, wxEXPAND | wxALL, 10);
        categoryPanel->SetSizer(categorySizer);
        _notebook->AddPage(categoryPanel, "Categories");

        _mainSizer->Add(_notebook, 1, wxEXPAND | wxALL, 10);

        // Buttons (Always Visible)
        wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
        wxButton *addAccountButton = new wxButton(this, wxID_ANY, "Add Account");
        buttonSizer->Add(addAccountButton, 1, wxEXPAND | wxALL, 10);
        addAccountButton->Bind(wxEVT_BUTTON, &home::onAddAccountButtonClicked, this);

        wxButton *addTransactionButton = new wxButton(this, wxID_ANY, "Add Transaction");
        buttonSizer->Add(addTransactionButton, 1, wxEXPAND | wxALL, 10);
        addTransactionButton->Bind(wxEVT_BUTTON, &home::onAddTransactionButtonClicked, this);

        wxButton *addCategoryButton = new wxButton(this, wxID_ANY, "Add Category");
        buttonSizer->Add(addCategoryButton, 1, wxEXPAND | wxALL, 10);
        addCategoryButton->Bind(wxEVT_BUTTON, &home::onAddCategoryButtonClicked, this);

        _mainSizer->Add(buttonSizer, 0, wxEXPAND | wxALL, 10);
        SetSizerAndFit(_mainSizer);
        Centre();
        Maximize();

        // Update views
        updateAccounts();
        updateCategories();
    }

private:
    wxBoxSizer *_mainSizer;
    wxNotebook *_notebook;
    wxNotebook *_accountNotebook;
    wxScrolledWindow *_categoryScrollWindow;
    wxBoxSizer *_categorySizer;
    std::unordered_map<std::string, wxWindow *> _accountTypePanels;
    std::unordered_map<std::string, wxGridSizer *> _accountTypeGrids;
    std::unordered_map<std::string, int> _accountTypeNumCols;
    std::unordered_map<std::string, wxWindow *> _accountViews;
    std::unordered_map<std::string, wxWindow *> _categoryViews;

    model_manager &_models = model_manager::getInstance();

    void updateAccounts()
    {
        for (auto account : _models.getAccounts())
        {
            std::string type = account.second->getType();

            if (_accountTypePanels.find(type) == _accountTypePanels.end())
            {
                auto panel = new wxScrolledWindow(_accountNotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL | wxHSCROLL);
                panel->SetScrollRate(5, 5);
                wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
                wxGridSizer *gridSizer = new wxGridSizer(1, 10, 10);
                panelSizer->Add(gridSizer, 0, wxEXPAND | wxALL, 10);
                panel->SetSizer(panelSizer);
                _accountNotebook->AddPage(panel, type);
                _accountTypePanels[type] = panel;
                _accountTypeGrids[type] = gridSizer;
            }

            if (!_accountViews.count(account.first))
            {
                auto accountView = model_view_factory::create(_accountTypePanels[type], account.second);
                if (accountView == nullptr)
                    continue;
                accountView->Fit();
                int columnWidth = accountView->GetSize().GetWidth();
                int parentWidth = _accountTypePanels[type]->GetClientSize().GetWidth();
                int numColumns = std::max(1, parentWidth / columnWidth);
                if (_accountTypeNumCols.find(type) == _accountTypeNumCols.end())
                {
                    _accountTypeNumCols[type] = numColumns;
                }
                else
                {
                    if (_accountTypeNumCols[type] > numColumns)
                    {
                        _accountTypeNumCols[type] = numColumns;
                    }
                }
                _accountTypeGrids[type]->SetCols(_accountTypeNumCols[type]);
                _accountTypeGrids[type]->Add(accountView, 0, wxEXPAND | wxALL, 10);
                _accountViews[account.first] = accountView;
            }
        }

        // Adjust layout
        for (auto &panel : _accountTypePanels)
        {
            panel.second->Layout();
        }
        _accountNotebook->Layout();
    }

    void updateCategories()
    {
        for (auto category : _models.getCategories())
        {
            if (!_categoryViews.count(category.first))
            {
                auto categoryView = model_view_factory::create(_categoryScrollWindow, category.second);
                _categorySizer->Add(categoryView, 0, wxEXPAND | wxALL, 10);
                _categoryViews[category.first] = categoryView;
            }
        }
        _categoryScrollWindow->FitInside();
        _categoryScrollWindow->Layout();
    }

    void onAddAccountButtonClicked(wxCommandEvent &event)
    {
        wxDialog *dialog = new wxDialog(this, wxID_ANY, "Select Account Type", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
        wxBoxSizer *dialogSizer = new wxBoxSizer(wxVERTICAL);

        wxArrayString accountTypes;
        accountTypes.Add("Bank Account");
        accountTypes.Add("Borrow Account");
        accountTypes.Add("Give Account");
        accountTypes.Add("Credit Card");
        accountTypes.Add("RD Account");
        accountTypes.Add("Chit Account");
        accountTypes.Add("Stock Account");

        wxChoice *choice = new wxChoice(dialog, wxID_ANY, wxDefaultPosition, wxDefaultSize, accountTypes);
        choice->SetSelection(0);
        dialogSizer->Add(choice, 0, wxEXPAND | wxALL, 10);

        wxButton *nextButton = new wxButton(dialog, wxID_OK, "Next");
        dialogSizer->Add(nextButton, 0, wxALIGN_CENTER | wxALL, 10);

        dialog->SetSizer(dialogSizer);
        dialog->Centre();

        if (dialog->ShowModal() == wxID_OK)
        {
            wxString selectedType = choice->GetStringSelection();
            wxDialog *accountDialog = new wxDialog(this, wxID_ANY, "Add Account", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
            wxBoxSizer *accountSizer = new wxBoxSizer(wxVERTICAL);

            auto accountForm = new AddModelForm(accountDialog, selectedType.ToStdString());
            accountSizer->Add(accountForm, 1, wxEXPAND | wxALL, 5);

            accountDialog->SetSizer(accountSizer);
            accountDialog->Centre();
            accountDialog->ShowModal();
        }
        updateAccounts();
    }

    void onAddTransactionButtonClicked(wxCommandEvent &event)
    {
        wxDialog *transactionDialog = new wxDialog(this, wxID_ANY, "Add Transaction", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
        wxBoxSizer *transactionSizer = new wxBoxSizer(wxVERTICAL);

        auto transactionForm = new AddModelForm(transactionDialog, "Transaction");
        transactionSizer->Add(transactionForm, 1, wxEXPAND | wxALL, 5);

        transactionDialog->SetSizer(transactionSizer);
        transactionDialog->Centre();
        transactionDialog->ShowModal();
    }

    void onAddCategoryButtonClicked(wxCommandEvent &event)
    {
        wxDialog *categoryDialog = new wxDialog(this, wxID_ANY, "Add Category", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
        wxBoxSizer *categorySizer = new wxBoxSizer(wxVERTICAL);

        auto categoryForm = new AddModelForm(categoryDialog, "Category");
        categorySizer->Add(categoryForm, 1, wxEXPAND | wxALL, 5);

        categoryDialog->SetSizer(categorySizer);
        categoryDialog->Centre();
        categoryDialog->ShowModal();
        updateCategories();
    }
};

#endif // HOME_HPP