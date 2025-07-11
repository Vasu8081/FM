#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <models/model.hpp>
#include <memory>
#include <wx/datetime.h>

class model_manager;
class Account;
class Category;

class Transaction : public Model
{
public:
    Transaction() = default;

    std::string getType() const override { return "Transaction"; }

    json toJson() const override;
    void fromJson(const json &j) override;

    std::vector<std::pair<std::string, std::string>> inputFormFields() const override;

    // Getters
    std::shared_ptr<Account> getFromAccount() const;
    std::shared_ptr<Account> getToAccount() const;
    std::shared_ptr<Category> getCategory() const;
    std::string getDescription() const;
    wxDateTime getDate() const;
    double getAmount() const;
    double getQuantity() const;
    double getInterestRate() const;

    // Setters
    void setFromAccount(std::shared_ptr<Account> from_account_id);
    void setToAccount(std::shared_ptr<Account> to_account_id);
    void setCategory(std::shared_ptr<Category> category_id);
    void setDescription(std::string description);
    void setDate(wxDateTime date);
    void setAmount(double amount);
    void setQuantity(double quantity);
    void setInterestRate(double interest_rate);

protected:
    std::shared_ptr<Account> _from_account_id;
    std::shared_ptr<Account> _to_account_id;
    std::shared_ptr<Category> _category_id;
    std::string _description;
    wxDateTime _date;
    double _amount;
    double _quantity = 0.0;
    double _interest_rate = 0.0;
};

#endif // TRANSACTION_HPP
