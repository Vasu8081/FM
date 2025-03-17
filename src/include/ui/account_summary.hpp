#ifndef ACCOUNT_SUMMARY_HPP
#define ACCOUNT_SUMMARY_HPP

#include <wx/wx.h>
#include <wx/grid.h>
#include <models/models_manager.hpp>
#include <formatter.hpp>

class AccountSummaryPanel : public wxPanel {
public:
    AccountSummaryPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY) {
        mainSizer = new wxBoxSizer(wxVERTICAL);
        SetSizer(mainSizer);
        accountSummaryView();
    }

private:
    wxBoxSizer* mainSizer;
    model_manager &_models = model_manager::getInstance();

    void accountSummaryView() {
        // Clear previous data
        mainSizer->Clear(true);

        // Calculate Values
        auto accounts = _models.getAccounts();
        std::unordered_map<std::string, std::pair<std::string, double>> accountValues;
        std::unordered_map<std::string, wxColour> accountColors;
        
        double portfolioValue = 0.0;
        double investmentValue = 0.0;
        double savingsValue = 0.0;
        double debtValue = 0.0;

        for (auto& account_ : accounts) {
            auto account = account_.second;
            auto portfolio = account->portfolioValue();
            if (accountValues.find(account->getType()) == accountValues.end()) {
                accountValues[account->getType()] = {portfolio.first, 0};
            }
            accountValues[account->getType()].second += portfolio.second;
            if(account->isSavingsAccount() || account->isInvestmentAccount()) {
                portfolioValue += portfolio.second;
            } else {
                portfolioValue -= portfolio.second;
            }

            if(account->isInvestmentAccount()){
                accountColors[account->getType()] = wxColour(233, 150, 91);
                investmentValue += portfolio.second;
            } else if(account->isSavingsAccount()){
                accountColors[account->getType()] = wxColour(50, 168, 82);
                savingsValue += portfolio.second;
            } else if(account->isDebtAccount()){
                accountColors[account->getType()] = wxColour(220, 77, 77);
                debtValue += portfolio.second;
            }
        }

        wxStaticBox* portfolioSummary = new wxStaticBox(this, wxID_ANY, "Portfolio Summary");
        auto portfolioSummarySizer = new wxStaticBoxSizer(portfolioSummary, wxVERTICAL);

        auto portfolioGrid = new wxGridSizer(4, wxSize(10, 10));
        std::unordered_map<std::string, wxColour> portfolioColors = {
            {"Portfolio Value", wxColour(64, 64, 64)},
            {"Investment Value", wxColour(233, 150, 91)},
            {"Savings Value", wxColour(50, 168, 82)},
            {"Debt Value", wxColour(220, 77, 77)}
        };
        std::vector<std::pair<std::string, double>> portfolioValues = {
            {"Portfolio Value", portfolioValue},
            {"Investment Value", investmentValue},
            {"Savings Value", savingsValue},
            {"Debt Value", debtValue}
        };

        for (const auto& portfolioValue : portfolioValues) {
            wxStaticBox* ptbox = new wxStaticBox(this, wxID_ANY, portfolioValue.first);
            auto sze = new wxStaticBoxSizer(ptbox, wxVERTICAL);
            ptbox->SetBackgroundColour(portfolioColors[portfolioValue.first]);
            auto text = new wxStaticText(this, wxID_ANY, Formatter::Amount(portfolioValue.second));
            text->SetForegroundColour(*wxWHITE);
            sze->Add(text, 0, wxALL, 5);
            portfolioGrid->Add(sze, 0, wxEXPAND | wxTOP | wxBOTTOM, 5);
        }

        portfolioSummarySizer->Add(portfolioGrid, 0, wxEXPAND | wxALL, 10);
        mainSizer->Add(portfolioSummarySizer, 0, wxEXPAND | wxALL, 10);

        // Create a static box for "Account Summary"
        wxStaticBox* summaryBox = new wxStaticBox(this, wxID_ANY, "Account Summary");
        auto accountSummarySizer = new wxStaticBoxSizer(summaryBox, wxVERTICAL);

        // Create a grid with 4 columns for account summaries
        auto gridSizer = new wxGridSizer(4, wxSize(10, 10));

        for (const auto& accountValue : accountValues) {
            wxStaticBox* accountBox = new wxStaticBox(this, wxID_ANY, accountValue.first);
            auto accountBoxSizer = new wxStaticBoxSizer(accountBox, wxVERTICAL);
            accountBox->SetBackgroundColour(accountColors[accountValue.first]);
            std::string totalValueText = accountValue.second.first + ": " + Formatter::Amount(accountValue.second.second);
            auto text = new wxStaticText(this, wxID_ANY, totalValueText);
            text->SetForegroundColour(*wxWHITE);
            accountBoxSizer->Add(text, 0, wxALL, 5);

            gridSizer->Add(accountBoxSizer, 1, wxEXPAND | wxTOP | wxBOTTOM, 10);
        }

        // Add grid inside the "Account Summary" box
        accountSummarySizer->Add(gridSizer, 0, wxEXPAND | wxALL, 10);

        // Add account summary to the main sizer
        mainSizer->Add(accountSummarySizer, 0, wxEXPAND | wxALL, 10);

        // Refresh Button
        auto refreshButton = new wxButton(this, wxID_ANY, "Refresh");
        mainSizer->Add(refreshButton, 0, wxALIGN_CENTER | wxALL, 10);

        // Bind the refresh event
        Bind(wxEVT_BUTTON, &AccountSummaryPanel::OnRefresh, this, refreshButton->GetId());

        // Refresh layout
        Layout();
    }

    void OnRefresh(wxCommandEvent& event) {
        accountSummaryView();
    }
};

#endif // ACCOUNT_SUMMARY_HPP
