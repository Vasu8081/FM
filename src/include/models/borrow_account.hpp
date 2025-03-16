#ifndef BORROW_ACCOUNT_H
#define BORROW_ACCOUNT_H

#include <string>
#include <models/account.hpp>
#include <wx/datetime.h>

class BorrowAccount : public Account
{
public:
    BorrowAccount()
    {
        _background_color = wxColour(58, 80, 107);
        _foreground_color = wxColour(235, 235, 235);
    }

    std::string getID() const override
    {
        return "BG." + _name;
    }

    std::string getType() const override
    {
        return "Borrow Account";
    }

    std::pair<std::string, double> portfolioValue() const override { return {"Borrowed Amount", _borrowed_amount}; }

    bool affectsPositivelyOnPortfolio() const override { return false; }

    bool isDebtAccount() const override { return true; }

    json toJson() const override
    {
        json j = {
            {"ID", getID()},
            {"Borrower", _name},
            {"UPI ID", _upi_id},
            {"Borrowed Amount", _borrowed_amount}};
        if (_borrowed_date.IsValid())
        {
            j["Borrowed Date"] = _borrowed_date.FormatISODate();
        }
        if (_due_date.IsValid())
        {
            j["Due Date"] = _due_date.FormatISODate();
        }
        return j;
    }

    void fromJson(const json &j) override
    {
        if (j.contains("Borrower"))
            _name = j.at("Borrower").get<std::string>();
        if (j.contains("UPI ID"))
            _upi_id = j.at("UPI ID").get<std::string>();

        if (j.contains("Borrowed Date"))
        {
            wxDateTime temp;
            temp.ParseISODate(j.at("Borrowed Date").get<std::string>());
            if (temp.IsValid())
            {
                _borrowed_date = temp;
            }
        }

        if (j.contains("Due Date"))
        {
            wxDateTime temp;
            temp.ParseISODate(j.at("Due Date").get<std::string>());
            if (temp.IsValid())
            {
                _due_date = temp;
            }
        }
    }

    std::vector<std::pair<std::string, std::string>> inputFormFields() const override
    {
        return {
            {"Borrower", "string"},
            {"UPI ID", "string"},
            {"Borrowed Date", "date"},
            {"Due Date", "date"}};
    }

    std::vector<std::pair<std::string, std::string>> displayFormFields() const override
    {
        return {
            {"header", _name},
            {"Total Borrowed Amount", Formatter::Amount(_borrowed_amount)},
            {"Due Date", Formatter::DateMonthYear(_due_date)}};
    }

    std::set<std::string> boldFormFields() const override
    {
        return {"header", "Total Borrowed Amount"};
    }

    std::unordered_map<std::string, wxColour> overrideFormColors() const override
    {
        return {
            {"Total Borrowed Amount", wxColour(235, 170, 235)}};
    }

    void amountIn(std::shared_ptr<Transaction> t) override
    {
        _borrowed_amount -= t->getAmount();
        notifyObservers();
    }

    void amountOut(std::shared_ptr<Transaction> t) override
    {
        _borrowed_amount += t->getAmount();
        notifyObservers();
    }

private:
    std::string _upi_id;
    double _borrowed_amount = 0.0;
    wxDateTime _borrowed_date;
    wxDateTime _due_date;
};

#endif
