#ifndef BANK_ACCOUINT_H
#define BANK_ACCOUINT_H

#include <string>
#include <models/account.hpp>

class BankAccount : public Account {
public:
    BankAccount(){
        _background_color = wxColour(40, 54, 73, 1); //muted teal
        _foreground_color = wxColour(220, 220, 220, 1); // off white
    }

    double portfolioValue() const override { return _balance; }

    std::string getID() const override {
        return "BA."+_bank_name+"."+_account_number;
    }

    std::string getType() const override {
        return "Bank Account";
    }

    json toJson() const override {
        json j = {
            {"ID", getID()},
            {"Bank Name", _bank_name},
            {"Balance", _balance},
            {"Account Number", _account_number},
            {"Branch", _branch},
            {"IFSC", _ifsc}
        };

        return j;
    }

    void fromJson(const json& j) override {
        if(j.contains("Bank Name")) _bank_name = j.at("Bank Name").get<std::string>(), _name = j.at("Bank Name").get<std::string>();
        if(j.contains("Account Number")) _account_number = j.at("Account Number").get<std::string>();
        if(j.contains("Branch")) _branch = j.at("Branch").get<std::string>();
        if(j.contains("IFSC")) _ifsc = j.at("IFSC").get<std::string>();
    }

    std::unordered_map<std::string, std::string> inputFormFields() const override {
        return {
            {"Bank Name", "string"},
            {"Account Number", "string"},
            {"Branch", "string"},
            {"IFSC", "string"}
        };
    }

    std::unordered_map<std::string, std::string> displayFormFields() const override {
        return {
            {_bank_name, std::to_string(_balance)},
            {"Account Number", _account_number},
            {"Branch", _branch},
            {"IFSC", _ifsc}
        };
    }

    void amountIn(double amount) override {
        _balance = _balance + amount;
        notifyObservers();
    }

    void amountOut(double amount) override {
        _balance = _balance - amount;
        notifyObservers();
    }

private:
    std::string _account_number;
    std::string _bank_name;
    std::string _branch;
    std::string _ifsc;
    double _balance = 0.0;
};

#endif // BANK_ACCOUINT_H