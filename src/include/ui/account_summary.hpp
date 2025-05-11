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
    model_manager& _models = model_manager::getInstance();

    void accountSummaryView() {
        mainSizer->Clear(true);

        auto accounts = _models.getAccounts();
        std::unordered_map<std::string, std::pair<std::string, double>> accountValues;
        std::unordered_map<std::string, wxColour> accountColors;

        double portfolioValue = 0.0, investmentValue = 0.0, savingsValue = 0.0, debtValue = 0.0;

        for (auto& account_ : accounts) {
            auto account = account_.second;
            auto portfolio = account->portfolioValue();

            accountValues[account->getType()].first = portfolio.first;
            accountValues[account->getType()].second += portfolio.second;

            if (account->isSavingsAccount() || account->isInvestmentAccount())
                portfolioValue += portfolio.second;
            else
                portfolioValue -= portfolio.second;

            if (account->isInvestmentAccount()) {
                accountColors[account->getType()] = wxColour(233, 150, 91);
                investmentValue += portfolio.second;
            } else if (account->isSavingsAccount()) {
                accountColors[account->getType()] = wxColour(50, 168, 82);
                savingsValue += portfolio.second;
            } else if (account->isDebtAccount()) {
                accountColors[account->getType()] = wxColour(220, 77, 77);
                debtValue += portfolio.second;
            }
        }

        // Portfolio Summary Section
        wxStaticBox* portfolioBox = new wxStaticBox(this, wxID_ANY, "Portfolio Summary");
        auto portfolioSizer = new wxStaticBoxSizer(portfolioBox, wxVERTICAL);
        wxGridSizer* portfolioGrid = new wxGridSizer(4, wxSize(10, 10));

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

        for (const auto& val : portfolioValues) {
            wxPanel* panel = new wxPanel(this);
            panel->SetBackgroundColour(portfolioColors[val.first]);
            wxBoxSizer* panelSizer = new wxBoxSizer(wxVERTICAL);

            auto text = new wxStaticText(panel, wxID_ANY, Formatter::Amount(val.second));
            text->SetForegroundColour(*wxWHITE);
            panelSizer->Add(text, 0, wxALL, 10);
            panel->SetSizer(panelSizer);

            wxStaticBoxSizer* wrapperSizer = new wxStaticBoxSizer(wxVERTICAL, this, val.first);
            wrapperSizer->Add(panel, 1, wxEXPAND);
            portfolioGrid->Add(wrapperSizer, 1, wxEXPAND | wxALL, 5);
        }

        portfolioSizer->Add(portfolioGrid, 0, wxEXPAND | wxALL, 10);
        mainSizer->Add(portfolioSizer, 0, wxEXPAND | wxALL, 10);

        // Account Summary Section
        wxStaticBox* accountBox = new wxStaticBox(this, wxID_ANY, "Account Summary");
        auto accountSizer = new wxStaticBoxSizer(accountBox, wxVERTICAL);
        wxGridSizer* accountGrid = new wxGridSizer(4, wxSize(10, 10));

        for (const auto& acc : accountValues) {
            wxPanel* panel = new wxPanel(this);
            panel->SetBackgroundColour(accountColors[acc.first]);
            wxBoxSizer* panelSizer = new wxBoxSizer(wxVERTICAL);

            std::string textStr = acc.second.first + ": " + Formatter::Amount(acc.second.second);
            auto text = new wxStaticText(panel, wxID_ANY, textStr);
            text->SetForegroundColour(*wxWHITE);
            panelSizer->Add(text, 0, wxALL, 10);
            panel->SetSizer(panelSizer);

            wxStaticBoxSizer* wrapperSizer = new wxStaticBoxSizer(wxVERTICAL, this, acc.first);
            wrapperSizer->Add(panel, 1, wxEXPAND);
            accountGrid->Add(wrapperSizer, 1, wxEXPAND | wxALL, 5);
        }

        accountSizer->Add(accountGrid, 0, wxEXPAND | wxALL, 10);
        mainSizer->Add(accountSizer, 0, wxEXPAND | wxALL, 10);

        // Refresh Button
        auto refreshButton = new wxButton(this, wxID_ANY, "Refresh");
        mainSizer->Add(refreshButton, 0, wxALIGN_CENTER | wxALL, 10);
        Bind(wxEVT_BUTTON, &AccountSummaryPanel::OnRefresh, this, refreshButton->GetId());

        Layout();
    }

    void OnRefresh(wxCommandEvent& event) {
        accountSummaryView();
    }
};

#endif // ACCOUNT_SUMMARY_HPP
