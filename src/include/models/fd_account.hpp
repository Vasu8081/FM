#ifndef FD_ACCOUNT_HPP
#define FD_ACCOUNT_HPP

#include <string>
#include <models/account.hpp>
#include <wx/datetime.h>

class FdAccount : public Account
{
public:
    FdAccount()
    {
        _background_color = wxColour(95, 85, 75);
        _foreground_color = wxColour(245, 238, 230);
    }

    std::string getID() const override
    {
        return "FD." + _name;
    }

    std::string getType() const override
    {
        return "FD";
    }

    std::pair<std::string, double> portfolioValue() const override { 
        if(wxDateTime::Now() < _maturity_date)
            return {"Invested Amount", _paid_amount};
        else
            return {"Invested Amount", _maturity_amount};
    }

    bool isInvestmentAccount() const override { return true; }

    json toJson() const override
    {
        json j = {
            {"ID", getID()},
            {"FD Name", _name},
            {"Paid Amount", _paid_amount},
            {"Maturity Amount", _maturity_amount},
            {"Start Date", _start_date.FormatISODate()},
            {"Maturity Date", _maturity_date.FormatISODate()}};

        return j;
    }

    void fromJson(const json &j) override
    {
        if (j.contains("FD Name"))
            _name = j.at("FD Name").get<std::string>();
        if (j.contains("Maturity Amount"))
            _maturity_amount = j.at("Maturity Amount").get<double>();
        if (j.contains("Start Date"))
            _start_date.ParseISODate(j.at("Start Date").get<std::string>());
        if (j.contains("Maturity Date"))
            _maturity_date.ParseISODate(j.at("Maturity Date").get<std::string>());
    }

    std::vector<std::pair<std::string, std::string>> inputFormFields() const override
    {
        return {
            {"FD Name", "string"},
            {"Maturity Amount", "double"},
            {"Start Date", "date"},
            {"Maturity Date", "date"}};
    }

    std::vector<std::pair<std::string, std::string>> displayFormFields() const override
    {
        auto months = Calculator::calculateMonthsDifference(_start_date, _maturity_date);
        auto yearly_interest = Calculator::findFDInterestRate(_paid_amount, _maturity_amount, months);
        return {
            {"header", _name},
            {"Maturity Amount", Formatter::Amount(_maturity_amount)},
            {"Start Date", Formatter::MonthYear(_start_date)},
            {"Tenure (in months)", Formatter::Integer(months)},
            {"Annual Interest Rate", Formatter::Percentage(yearly_interest)},
            {"Amount Paid", Formatter::Amount(_paid_amount)}};
    }

    std::set<std::string> boldFormFields() const override
    {
        return {"header", "Maturity Amount"};
    }

    std::unordered_map<std::string, wxColour> overrideFormColors() const override
    {
        return {};
        return {
            {"Maturity Amount", wxColour(235, 170, 235)}};
    }

    void amountIn(std::shared_ptr<Transaction> t) override
    {
        _paid_amount = _paid_amount + t->getAmount();
        notifyObservers();
    }

    void amountOut(std::shared_ptr<Transaction> t) override
    {
        _paid_amount = _paid_amount - t->getAmount();
        notifyObservers();
    }

private:
    double _monthly_payment;
    double _paid_amount;
    double _maturity_amount;
    wxDateTime _start_date;
    wxDateTime _maturity_date;
};

#endif