#ifndef RD_ACCOUNT_HPP
#define RD_ACCOUNT_HPP

#include <string>
#include <models/account.hpp>
#include <wx/datetime.h>

class RdAccount : public Account {
public:
    RdAccount(){
        _background_color = wxColour(85, 70, 65, 1);
        _foreground_color = wxColour(245, 230, 225, 1);
    }

    std::string getID() const override {
        return "RD."+_name;
    }

    std::string getType() const override {
        return "RD";
    }

    double portfolioValue() const override { return _paid_amount; }

    json toJson() const override {
        json j = {
            {"ID", getID()},
            {"RD Name", _name},
            {"Monthly Payment", _monthly_payment},
            {"Paid Amount", _paid_amount},
            {"Maturity Amount", _maturity_amount},
            {"Start Date", _start_date.FormatISODate()},
            {"Monthly Payment Date", _monthly_payment_date.FormatISODate()},
            {"Maturity Date", _maturity_date.FormatISODate()}
        };

        return j;
    }

    void fromJson(const json& j) override {
        if(j.contains("RD Name")) _name = j.at("RD Name").get<std::string>();
        if(j.contains("Monthly Payment")) _monthly_payment = j.at("Monthly Payment").get<double>();
        if(j.contains("Maturity Amount")) _maturity_amount = j.at("Maturity Amount").get<double>();
        if(j.contains("Start Date")) _start_date.ParseISODate(j.at("Start Date").get<std::string>());
        if(j.contains("Monthly Payment Date")) _monthly_payment_date.ParseISODate(j.at("Monthly Payment Date").get<std::string>());
        if(j.contains("Maturity Date")) _maturity_date.ParseISODate(j.at("Maturity Date").get<std::string>());
    }

    std::unordered_map<std::string, std::string> inputFormFields() const override {
        return {
            {"RD Name", "string"},
            {"Monthly Payment", "double"},
            {"Maturity Amount", "double"},
            {"Start Date", "date"},
            {"Monthly Payment Date", "date"},
            {"Maturity Date", "date"}
        };
    }

    std::unordered_map<std::string, std::string> displayFormFields() const override {
        return {
            {"header", _name},
            {"Monthly Payment", std::to_string(_monthly_payment)},
            {"Paid Amount", std::to_string(_paid_amount)},
            {"Maturity Amount", std::to_string(_maturity_amount)},
            {"Start Date", _start_date.FormatISODate().ToStdString()},
            {"Monthly Payment Date", _monthly_payment_date.FormatISODate().ToStdString()},
            {"Maturity Date", _maturity_date.FormatISODate().ToStdString()}
        };
    }

    std::set<std::string> boldFormFields() const override {
        return { "header", "Paid Amount" };
    }

    std::unordered_map<std::string, wxColour> overrideFormColors() const override {
        return {
            {"Paid Amount", wxColour(235, 170, 235)}
        };
    }


    void amountIn(double amount) override {
        _paid_amount = _paid_amount + amount;
        notifyObservers();
    }

    void amountOut(double amount) override {
        _paid_amount = _paid_amount - amount;
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