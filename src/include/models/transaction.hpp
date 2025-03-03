#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <models/model.hpp>
#include <memory>
#include <wx/datetime.h>

class model_manager;
class account;
class category;

class transaction : public model {
public:
    transaction() = default;

    transaction(std::shared_ptr<account> from_account_id, 
                std::shared_ptr<account> to_account_id, 
                std::shared_ptr<category> category_id, 
                std::string description,
                wxDateTime date, 
                double amount);

    json toJson() const override;
    void fromJson(const json& j) override;

    std::unordered_map<std::string, std::string> fieldTypes() const override;

    // Getters
    std::shared_ptr<account> getFromAccount() const;
    std::shared_ptr<account> getToAccount() const;
    std::shared_ptr<category> getCategory() const;
    std::string getDescription() const;
    wxDateTime getDate() const;
    double getAmount() const;

    // Setters
    void setFromAccount(std::shared_ptr<account> from_account_id);
    void setToAccount(std::shared_ptr<account> to_account_id);
    void setCategory(std::shared_ptr<category> category_id);
    void setDescription(std::string description);
    void setDate(wxDateTime date);
    void setAmount(double amount);

protected:
    std::shared_ptr<account> _from_account_id;
    std::shared_ptr<account> _to_account_id;
    std::shared_ptr<category> _category_id;
    std::string _description;
    wxDateTime _date;
    double _amount;
};

#endif // TRANSACTION_HPP
