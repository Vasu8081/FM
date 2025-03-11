#ifndef BANK_ACCOUINT_H
#define BANK_ACCOUINT_H

#include <string>
#include <models/account.hpp>

class BankAccount : public Account {
public:
    BankAccount(){
        _background_color = wxColour(90, 100, 90);
        _foreground_color = wxColour(235, 245, 235);
    }

    double portfolioValue() const override { return _balance; }

    std::string getID() const override {
        return "BA."+_name+"."+_account_number;
    }

    std::string getType() const override {
        return "Bank Account";
    }

    json toJson() const override {
        json j = {
            {"ID", getID()},
            {"Bank Name", _name},
            {"Balance", _balance},
            {"Account Number", _account_number},
            {"Branch", _branch},
            {"IFSC", _ifsc}
        };

        return j;
    }

    void fromJson(const json& j) override {
        if(j.contains("Bank Name")) _name = j.at("Bank Name").get<std::string>();
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
            {"header", _name},
            {"Balance", Amount(_balance)},
            {"Account Number", _account_number},
            {"Branch", _branch},
            {"IFSC", _ifsc}
        };
    }

    std::set<std::string> boldFormFields() const override {
        return { "header", "Balance" };
    }

    std::unordered_map<std::string, wxColour> overrideFormColors() const override {
        return {
            {"Balance", wxColour(235, 170, 235)}
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
    std::string _branch;
    std::string _ifsc;
    double _balance = 0.0;
};

#endif // BANK_ACCOUINT_H