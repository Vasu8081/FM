#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

#include <models/model.hpp>
#include <string>
#include <unordered_map>
#include <models/transaction.hpp>

class account : public virtual model {
public:
    account() = default;
    account(std::string id, std::string name, double balance) : _id(id), _name(name), _balance(balance) {
        _from_transactions = std::vector<std::shared_ptr<transaction>>();
        _to_transactions = std::vector<std::shared_ptr<transaction>>();
    }
    virtual ~account() = default;

    virtual std::string generateID() const = 0;

    virtual nlohmann::json to_json() const override{
        nlohmann::json j = {
            {"id", _id},
            {"name", _name},
            {"balance", _balance}
        };

        return j;
    }

    virtual void from_json(const nlohmann::json& j) {
        _id = j.at("id").get<std::string>();
        _name = j.at("name").get<std::string>();
        _balance = j.at("balance").get<double>();
    }

    virtual std::string to_str() {
        return "ID: " + _id + ", Name: " + _name + ", Balance: " + std::to_string(_balance);
    }

    //Getters
    std::string getID() const;
    std::string getName() const;
    double getBalance() const;

    //Setters
    void setBalance(double balance) { _balance = balance; }
    void addFromTransaction(std::shared_ptr<transaction> t) { _from_transactions.push_back(t); }
    void addToTransaction(std::shared_ptr<transaction> t) { _to_transactions.push_back(t); }

protected:
    std::string _id;
    std::string _name;
    double _balance;
    std::vector<std::shared_ptr<transaction>> _from_transactions;
    std::vector<std::shared_ptr<transaction>> _to_transactions;
};

#endif // ACCOUNT_HPP