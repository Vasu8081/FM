#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

#include <models/model.hpp>
#include <string>
#include <unordered_map>
#include <models/transaction.hpp>
#include <wx/colour.h>
#include <set>
#include <vector>
#include <format>
#include <wx/wx.h>

class Account : public virtual Model {
public:
    Account() = default;
    virtual ~Account() = default;

    virtual std::string getID() const = 0;

    virtual std::string getType() const = 0;

    virtual nlohmann::json toJson() const override = 0;

    virtual void fromJson(const nlohmann::json& j) override = 0;

    virtual std::unordered_map<std::string, std::string> inputFormFields() const override = 0;

    virtual std::unordered_map<std::string, std::string> displayFormFields() const = 0;

    virtual std::set<std::string> boldFormFields() const = 0;

    virtual std::unordered_map<std::string, wxColour> overrideFormColors() const = 0;

    wxColour getForegroundColor() const { return _foreground_color; }
    wxColour getBackgroundColor() const { return _background_color; }

    //Behavior if this Account is a to Account during a Transaction
    virtual void amountIn(double amount) = 0;
    //Behavior if this Account is a from Account during a Transaction
    virtual void amountOut(double amount) = 0;
    // Get the value of the portfolio can be positive or negative based on how it contributes to overall portfolio value
    virtual double portfolioValue() const = 0;

    //Getters
    std::string getName() const { return _name; }

    //Setters
    void addFromTransaction(std::shared_ptr<Transaction> t) { 
        _from_transactions.push_back(t);
        amountOut(t->getAmount());
    }
    void addToTransaction(std::shared_ptr<Transaction> t) { 
        _to_transactions.push_back(t);
        amountIn(t->getAmount());
    }

    std::vector<std::shared_ptr<Transaction>> getFromTransactions() const { return _from_transactions; }
    std::vector<std::shared_ptr<Transaction>> getToTransactions() const { return _to_transactions; }

protected:
    std::string _name;
    wxColour _foreground_color;
    wxColour _background_color;
    std::vector<std::shared_ptr<Transaction>> _from_transactions;
    std::vector<std::shared_ptr<Transaction>> _to_transactions;
};

#endif // ACCOUNT_HPP