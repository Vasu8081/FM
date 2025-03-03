#ifndef BORROW_ACCOUNT_H
#define BORROW_ACCOUNT_H

#include <string>
#include <models/account.hpp>
#include <wx/datetime.h>

class borrow_account : public account {
public:
    borrow_account() : _borrowed_date(), _due_date() {}

    std::string generateID() const override {
        return "BG." + _name;
    }

    // Getters
    std::string getBorrower() const { return _name; }
    std::string getUpiId() const { return _upi_id; }
    double getBorrowedAmount() const { return _borrowed_amount; }
    wxDateTime getBorrowedDate() const { return _borrowed_date; }
    wxDateTime getDueDate() const { return _due_date; }

    // Inherited functions
    json toJson() const override {
        json j = {
            {"Id", _id},
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
        if(j.contains("Id")) _id = j.at("Id").get<std::string>();
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
        
        if(_id.empty()) _id = generateID();
    }

    std::unordered_map<std::string, std::string> fieldTypes() const override {
        return {
            {"Borrower", "string"},
            {"UPI ID", "string"},
            {"Borrowed Date", "date"},
            {"Due Date", "date"}
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
