#ifndef BORROW_ACCOUNT_H
#define BORROW_ACCOUNT_H

#include <string>
#include <models/account.hpp>
#include <wx/datetime.h>

class BorrowAccount : public Account {
public:
    BorrowAccount() {
        _background_color = wxColour(58, 80, 107);
        _foreground_color = wxColour(235, 235, 235);
    }

    std::string getID() const override {
        return "BG." + _name;
    }

    std::string getType() const override {
        return "Borrow Account";
    }

    double portfolioValue() const override { return -_borrowed_amount; }

    json toJson() const override {
        json j = {
            {"ID", getID()},
            {"Borrower", _name},
            {"UPI ID", _upi_id},
            {"Borrowed Amount", _borrowed_amount}
        };
        if (_borrowed_date.IsValid()) {
            j["Borrowed Date"] = _borrowed_date.FormatISODate();
        }
        if (_due_date.IsValid()) {
            j["Due Date"] = _due_date.FormatISODate();
        }
        return j;
    }

    void fromJson(const json& j) override {
        if(j.contains("Borrower")) _name = j.at("Borrower").get<std::string>();
        if(j.contains("UPI ID")) _upi_id = j.at("UPI ID").get<std::string>();
        
        if(j.contains("Borrowed Date")) {
            wxDateTime temp;
            temp.ParseISODate(j.at("Borrowed Date").get<std::string>());
            if (temp.IsValid()) {
                _borrowed_date = temp;
            }
        }

        if(j.contains("Due Date")) {
            wxDateTime temp;
            temp.ParseISODate(j.at("Due Date").get<std::string>());
            if (temp.IsValid()) {
                _due_date = temp;
            }
        }
    }

    std::unordered_map<std::string, std::string> inputFormFields() const override {
        return {
            {"Borrower", "string"},
            {"UPI ID", "string"},
            {"Borrowed Date", "date"},
            {"Due Date", "date"}
        };
    }

    std::unordered_map<std::string, std::string> displayFormFields() const override {
        return {
            {"Borrower", _name},
            {"UPI ID", _upi_id},
            {"Borrowed Amount", std::to_string(_borrowed_amount)},
            {"Borrowed Date", _borrowed_date.FormatISODate().ToStdString()},
            {"Due Date", _due_date.FormatISODate().ToStdString()}
        };
    }

    void amountIn(double amount) override {
        _borrowed_amount -= amount;
        notifyObservers();
    }

    void amountOut(double amount) override {
        _borrowed_amount += amount;
        notifyObservers();
    }

private:
    std::string _upi_id;
    double _borrowed_amount = 0.0;
    wxDateTime _borrowed_date;
    wxDateTime _due_date;
};

#endif
