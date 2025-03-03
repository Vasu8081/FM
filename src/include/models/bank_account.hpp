#ifndef BANK_ACCOUINT_H
#define BANK_ACCOUINT_H

#include <string>
#include <models/account.hpp>

class bank_account : public account {
public:
    bank_account() = default;

    std::string generateID() const override {
        return "BA."+_bank_name+"."+_account_number;
    }

    //Getters
    std::string getAccountNumber() const { return _account_number; }
    std::string getBankName() const { return _bank_name; }
    std::string getBranch() const { return _branch; }
    std::string getIfsc() const { return _ifsc; }
    double getBalance() const { return _balance; }

    //Inherited functions
    json toJson() const override {
        json j = {
            {"Id", _id},
            {"Bank Name", _bank_name},
            {"Balance", _balance},
            {"Account Number", _account_number},
            {"Branch", _branch},
            {"IFSC", _ifsc}
        };

        return j;
    }

    void fromJson(const json& j) override {
        if(j.contains("Id")) _id = j.at("Id").get<std::string>();
        if(j.contains("Bank Name")) _bank_name = j.at("Bank Name").get<std::string>(), _name = j.at("Bank Name").get<std::string>();
        if(j.contains("Account Number")) _account_number = j.at("Account Number").get<std::string>();
        if(j.contains("Branch")) _branch = j.at("Branch").get<std::string>();
        if(j.contains("IFSC")) _ifsc = j.at("IFSC").get<std::string>();
        if(_id.empty()) _id = generateID();
    }

    std::unordered_map<std::string, std::string> fieldTypes() const override {
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