#include <models/transaction.hpp>
#include <models/models_manager.hpp>
#include <models/account.hpp>
#include <models/category.hpp>

json Transaction::toJson() const
{
    json j;
    j["From Account"] = _from_account_id ? _from_account_id->getID() : "";
    j["To Account"] = _to_account_id ? _to_account_id->getID() : "";
    j["Category"] = _category_id ? _category_id->getID() : "";
    j["Description"] = _description;
    j["Date"] = _date.FormatISODate();
    j["Amount"] = _amount;
    j["Quantity"] = _quantity;
    j["Interest Rate"] = _interest_rate;
    return j;
}

void Transaction::fromJson(const json &j)
{
    model_manager &model_manager_ = model_manager::getInstance();

    if (j.contains("From Account"))
    {
        auto id = j["From Account"].get<std::string>();
        if (id != "")
        {
            _from_account_id = model_manager_.getAccounts()[id];
        }
    }

    if (j.contains("To Account"))
    {
        auto id = j["To Account"].get<std::string>();
        if (id != "")
        {
            _to_account_id = model_manager_.getAccounts()[id];
            if(_to_account_id->getType() == "Loan Account")
            {
                if(j.contains("Interest Rate"))
                {
                    _interest_rate = j["Interest Rate"].get<double>();
                }
                else
                {
                    auto la = std::dynamic_pointer_cast<LoanAccount>(_to_account_id);
                    _interest_rate = la->getInterestRate();
                }
            }
        }
    }

    if (j.contains("Category"))
    {
        auto id = j["Category"].get<std::string>();
        if (id != "")
        {
            _category_id = model_manager_.getCategories()[id];
        }
    }

    if (j.contains("Description"))
    {
        _description = j["Description"].get<std::string>();
    }

    if (j.contains("Date"))
    {
        wxString date_str = j["Date"].get<std::string>();
        _date.ParseISODate(date_str);
    }

    if (j.contains("Amount"))
    {
        _amount = j["Amount"].get<double>();
    }

    if (j.contains("Quantity"))
    {
        _quantity = j["Quantity"].get<double>();
    }

    if(j.contains("Quantity (only for stock account)"))
    {
        _quantity = j["Quantity (only for stock account)"].get<double>();
    }
}

std::vector<std::pair<std::string, std::string>> Transaction::inputFormFields() const
{
    //From Account and To Account also used in add_model.cpp (if changed name here, need to change there also)
    return {
        {"From Account", "Account"},
        {"To Account", "Account"},
        {"Category", "Category"},
        {"Description", "string"},
        {"Date", "date"},
        {"Amount", "double"},
        {"Quantity (only for stock account)", "double"}};
}

// Getters
std::shared_ptr<Account> Transaction::getFromAccount() const { return _from_account_id; }
std::shared_ptr<Account> Transaction::getToAccount() const { return _to_account_id; }
std::shared_ptr<Category> Transaction::getCategory() const { return _category_id; }
std::string Transaction::getDescription() const { return _description; }
wxDateTime Transaction::getDate() const { return _date; }
double Transaction::getAmount() const { return _amount; }
double Transaction::getQuantity() const { return _quantity; }
double Transaction::getInterestRate() const { return _interest_rate; }

// Setters
void Transaction::setFromAccount(std::shared_ptr<Account> from_account_id) { _from_account_id = from_account_id; }
void Transaction::setToAccount(std::shared_ptr<Account> to_account_id) { _to_account_id = to_account_id; }
void Transaction::setCategory(std::shared_ptr<Category> category_id) { _category_id = category_id; }
void Transaction::setDescription(std::string description) { _description = description; }
void Transaction::setDate(wxDateTime date) { _date = date; }
void Transaction::setAmount(double amount) { _amount = amount; }
void Transaction::setQuantity(double quantity) { _quantity = quantity; }
void Transaction::setInterestRate(double interest_rate) { _interest_rate = interest_rate; }
