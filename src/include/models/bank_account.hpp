#ifndef BANK_ACCOUINT_H
#define BANK_ACCOUINT_H

#include <string>
#include <models/account.hpp>

class bank_account : public account {
public:
    bank_account();
    bank_account(const std::string& account_number, const std::string& bank_name, const std::string& branch, const std::string& ifsc, double balance) : account("", bank_name, balance) {
        _account_number = account_number;
        _bank_name = bank_name;
        _branch = branch;
        _ifsc = ifsc;
        _id = generateID();
    }

    std::string generateID() const override {
        return "BA."+_bank_name+"."+_account_number;
    }

    //Getters
    std::string getAccountNumber() const { return _account_number; }
    std::string getBankName() const { return _bank_name; }
    std::string getBranch() const { return _branch; }
    std::string getIfsc() const { return _ifsc; }

    //Setters
    void setAccountNumber(const std::string& account_number) { _account_number = account_number; }
    void setBankName(const std::string& bank_name) { _bank_name = bank_name; }
    void setBranch(const std::string& branch) { _branch = branch; }
    void setIfsc(const std::string& ifsc) { _ifsc = ifsc; }

    //Inherited functions
    json to_json() const override {
        json j = {
            {"id", _id},
            {"account_number", _account_number},
            {"bank_name", _bank_name},
            {"branch", _branch},
            {"ifsc", _ifsc},
            {"balance", _balance}
        };

        return j;
    }

    void from_json(const json& j) override {
        _id = j.at("id").get<std::string>();
        _account_number = j.at("account_number").get<std::string>();
        _bank_name = j.at("bank_name").get<std::string>();
        _branch = j.at("branch").get<std::string>();
        _ifsc = j.at("ifsc").get<std::string>();
        _balance = j.at("balance").get<double>();
    }

private:
    std::string _account_number;
    std::string _bank_name;
    std::string _branch;
    std::string _ifsc;
};

#endif // BANK_ACCOUINT_H