#ifndef RD_ACCOUNT_HPP
#define RD_ACCOUNT_HPP

#include <string>
#include <models/account.hpp>
#include <wx/datetime.h>

class RdAccount : public Account
{
public:
    RdAccount()
    {
        _background_color = wxColour(95, 85, 75);
        _foreground_color = wxColour(245, 238, 230);
    }

    std::string getID() const override
    {
        return "RD." + _name;
    }

    std::string getType() const override
    {
        return "RD";
    }

    double portfolioValue() const override { return _paid_amount; }

    json toJson() const override
    {
        json j = {
            {"ID", getID()},
            {"RD Name", _name},
            {"Monthly Payment", _monthly_payment},
            {"Paid Amount", _paid_amount},
            {"Maturity Amount", _maturity_amount},
            {"Start Date", _start_date.FormatISODate()},
            {"Monthly Payment Date", _monthly_payment_date.FormatISODate()},
            {"Maturity Date", _maturity_date.FormatISODate()}};

        return j;
    }

    void fromJson(const json &j) override
    {
        if (j.contains("RD Name"))
            _name = j.at("RD Name").get<std::string>();
        if (j.contains("Monthly Payment"))
            _monthly_payment = j.at("Monthly Payment").get<double>();
        if (j.contains("Maturity Amount"))
            _maturity_amount = j.at("Maturity Amount").get<double>();
        if (j.contains("Start Date"))
            _start_date.ParseISODate(j.at("Start Date").get<std::string>());
        if (j.contains("Monthly Payment Date"))
            _monthly_payment_date.ParseISODate(j.at("Monthly Payment Date").get<std::string>());
        if (j.contains("Maturity Date"))
            _maturity_date.ParseISODate(j.at("Maturity Date").get<std::string>());
    }

    std::vector<std::pair<std::string, std::string>> inputFormFields() const override
    {
        return {
            {"RD Name", "string"},
            {"Monthly Payment", "double"},
            {"Maturity Amount", "double"},
            {"Start Date", "date"},
            {"Monthly Payment Date", "date"},
            {"Maturity Date", "date"}};
    }

    std::vector<std::pair<std::string, std::string>> displayFormFields() const override
    {
        auto months = Calculator::calculateMonthsDifference(_start_date, _maturity_date);
        auto yearly_interest = Calculator::findRDInterestRate(_monthly_payment, _maturity_amount, months);
        return {
            {"header", _name},
            {"Monthly Payment", Formatter::Amount(_monthly_payment)},
            {"Total Expected Amount", Formatter::Amount(_maturity_amount)},
            {"Payment Due Date", Formatter::MonthlyPaymentDate(_monthly_payment_date)},
            {"First Installment", Formatter::MonthYear(_start_date)},
            {"Final Installment", Formatter::MonthYear(_maturity_date)},
            {"Tenure (in months)", Formatter::Integer(months)},
            {"Annual Interest Rate", Formatter::Percentage(yearly_interest)},
            {"Total Amount Paid", Formatter::Amount(_paid_amount)}};
    }

    std::set<std::string> boldFormFields() const override
    {
        return {"header", "Total Amount Paid"};
    }

    std::unordered_map<std::string, wxColour> overrideFormColors() const override
    {
        return {
            {"Total Amount Paid", wxColour(235, 170, 235)}};
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
    wxDateTime _monthly_payment_date;
};

#endif