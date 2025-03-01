#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

#include <models/model.hpp>
#include <string>
#include <unordered_map>

class account : public virtual model {
public:
    account() = default;
    account(std::string id, std::string name, double balance) : _id(id), _name(name), _balance(balance) {}
    virtual ~account() = default;

    virtual std::string generateID() const = 0;

    virtual nlohmann::json to_json() const {
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

    //Getters
    std::string getID() const;
    std::string getName() const;
    double getBalance() const;

    //Setters
    void setBalance(double balance) { _balance = balance; }

protected:
    std::string _id;
    std::string _name;
    double _balance;
};

#endif // ACCOUNT_HPP