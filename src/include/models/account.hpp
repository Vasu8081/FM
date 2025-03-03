#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

#include <models/model.hpp>
#include <string>
#include <unordered_map>
#include <models/transaction.hpp>

class account : public virtual model {
public:
    account() = default;
    account(std::string id, std::string name) : _id(id), _name(name) {
        _from_transactions = std::vector<std::shared_ptr<transaction>>();
        _to_transactions = std::vector<std::shared_ptr<transaction>>();
    }
    virtual ~account() = default;

    virtual std::string generateID() const = 0;

    virtual nlohmann::json toJson() const override{
        nlohmann::json j = {
            {"Id", _id},
            {"Name", _name}
        };

        return j;
    }

    virtual void fromJson(const nlohmann::json& j) override {
        _id = j.at("Id").get<std::string>();
        if(j.contains("Name")) _name = j.at("Name").get<std::string>();
        if(_id.empty()) _id = generateID();
    }

    virtual std::string toStr() override {
        return "ID: " + _id + ", Name: " + _name;
    }

    virtual std::unordered_map<std::string, std::string> fieldTypes() const override {
        return {
            {"Name", "string"},
            {"Balance", "double"}
        };
    }

    //Behavior if this account is a to account during a transaction
    virtual void amountIn(double amount) = 0;
    //Behavior if this account is a from account during a transaction
    virtual void amountOut(double amount) = 0;

    //Getters
    std::string getID() const { return _id; }
    std::string getName() const { return _name; }

    //Setters
    void addFromTransaction(std::shared_ptr<transaction> t) { _from_transactions.push_back(t); amountOut(t->getAmount()); }
    void addToTransaction(std::shared_ptr<transaction> t) { _to_transactions.push_back(t); amountIn(t->getAmount()); }

    std::vector<std::shared_ptr<transaction>> getFromTransactions() const { return _from_transactions; }
    std::vector<std::shared_ptr<transaction>> getToTransactions() const { return _to_transactions; }

protected:
    std::string _id;
    std::string _name;
    std::vector<std::shared_ptr<transaction>> _from_transactions;
    std::vector<std::shared_ptr<transaction>> _to_transactions;
};

#endif // ACCOUNT_HPP