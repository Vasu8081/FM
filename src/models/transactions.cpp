#include <models/transaction.hpp>
#include <models/models_manager.hpp>
#include <models/account.hpp>
#include <models/category.hpp>

transaction::transaction(std::shared_ptr<account> from_account_id, 
                         std::shared_ptr<account> to_account_id, 
                         std::shared_ptr<category> category_id, 
                         std::string description,
                         wxDateTime date, 
                         double amount)
    : _from_account_id(from_account_id), 
      _to_account_id(to_account_id), 
      _category_id(category_id), 
      _description(description), 
      _date(date), 
      _amount(amount) {}

json transaction::toJson() const {
    json j;
    j["From Account"] = _from_account_id ? _from_account_id->generateID() : "";
    j["To Account"] = _to_account_id ? _to_account_id->generateID() : "";
    j["Category"] = _category_id ? _category_id->generateID() : "";
    j["Description"] = _description;
    j["Date"] = _date.FormatISODate();
    j["Amount"] = _amount;
    return j;
}

void transaction::fromJson(const json& j) {
    model_manager& model_manager_ = model_manager::getInstance();

    if(j.contains("From Account")){
        auto id = j["From Account"].get<std::string>();
        if(id != "") {
            _from_account_id = model_manager_.getAccounts()[id];
        }
    }

    if(j.contains("To Account")){
        auto id = j["To Account"].get<std::string>();
        if(id != "") {
            _to_account_id = model_manager_.getAccounts()[id];
        }
    }

    if(j.contains("Category")){
        auto id = j["Category"].get<std::string>();
        if(id != "") {
            _category_id = model_manager_.getCategories()[id];
        }
    }

    if(j.contains("Description")){
        _description = j["Description"].get<std::string>();
    }

    if (j.contains("Date")) {
        wxString date_str = j["Date"].get<std::string>();
        _date.ParseISODate(date_str);
    }

    if(j.contains("Amount")){
        _amount = j["Amount"].get<double>();
    }  
}

std::string transaction::toStr() {
    std::string from_account_id = _from_account_id ? _from_account_id->generateID() : "External";
    std::string to_account_id = _to_account_id ? _to_account_id->generateID() : "External";
    std::string category_id = _category_id ? _category_id->generateID() : "Unknown";
    return from_account_id + " -> " + to_account_id + " (Rupees:" + std::to_string(_amount) + ")" + " [ Category: " + category_id + "]";
}

std::unordered_map<std::string, std::string> transaction::fieldTypes() const {
    return {
        {"From Account", "account"},
        {"To Account", "account"},
        {"Category", "category"},
        {"Description", "string"},
        {"Date", "date"},
        {"Amount", "double"}
    };
}

// Getters
std::shared_ptr<account> transaction::getFromAccount() const { return _from_account_id; }
std::shared_ptr<account> transaction::getToAccount() const { return _to_account_id; }
std::shared_ptr<category> transaction::getCategory() const { return _category_id; }
std::string transaction::getDescription() const { return _description; }
wxDateTime transaction::getDate() const { return _date; }
double transaction::getAmount() const { return _amount; }

// Setters
void transaction::setFromAccount(std::shared_ptr<account> from_account_id) { _from_account_id = from_account_id; }
void transaction::setToAccount(std::shared_ptr<account> to_account_id) { _to_account_id = to_account_id; }
void transaction::setCategory(std::shared_ptr<category> category_id) { _category_id = category_id; }
void transaction::setDescription(std::string description) { _description = description; }
void transaction::setDate(wxDateTime date) { _date = date; }
void transaction::setAmount(double amount) { _amount = amount; }
