#ifndef GIVE_ACCOUNT_H
#define GIVE_ACCOUNT_H

#include <string>
#include <models/account.hpp>
#include <wx/datetime.h>

class GiveAccount : public Account {
public:
    GiveAccount() : _given_date(), _due_date() {}

    std::string getID() const override {
        return "GV." + _name;
    }

    // Getters
    std::string getGivenTo() const { return _name; }
    double getGivenAmount() const { return _given_amount; }
    double portfolioValue() const override { return _given_amount; }
    wxDateTime getGivenDate() const { return _given_date; }
    wxDateTime getDueDate() const { return _due_date; }

    // Inherited functions
    json toJson() const override {
        json j = {
            {"ID", getID()},
            {"Given To", _name},
            {"Given Amount", _given_amount}
        };
        if (_given_date.IsValid()) {
            j["Given Date"] = _given_date.FormatISODate();
        }
        if (_due_date.IsValid()) {
            j["Due Date"] = _due_date.FormatISODate();
        }
        return j;
    }

    void fromJson(const json& j) override {
        if(j.contains("Given To")) _name = j.at("Given To").get<std::string>();
        
        if(j.contains("Given Date")) {
            wxDateTime temp;
            temp.ParseISODate(j.at("Given Date").get<std::string>());
            if (temp.IsValid()) {
                _given_date = temp;
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
            {"Given To", "string"},
            {"Given Date", "date"},
            {"Due Date", "date"}
        };
    }

    void amountIn(double amount) override {
        _given_amount += amount;
        notifyObservers();
    }

    void amountOut(double amount) override {
        _given_amount -= amount;
        notifyObservers();
    }

private:
    double _given_amount = 0.0;
    wxDateTime _given_date;
    wxDateTime _due_date;
};

#endif
