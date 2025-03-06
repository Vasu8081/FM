#ifndef BANK_ACCOUINT_H
#define BANK_ACCOUINT_H

#include <string>
#include <models/account.hpp>

class BankAccount : public Account {
public:
    BankAccount() = default;

    //Getters
    std::string getHeader() const { return _bank_name+": "+std::to_string(_balance); }
    std::string getBankName() const { return _bank_name; }
    double getBalance() const { return _balance; }
    double portfolioValue() const override { return _balance; }
    std::string getAccountNumber() const { return _account_number; }
    std::string getBranch() const { return _branch; }
    std::string getIfsc() const { return _ifsc; }
    

    //Inherited functions
    std::string getID() const override {
        return "BA."+_bank_name+"."+_account_number;
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