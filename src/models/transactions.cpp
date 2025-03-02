#include <models/transaction.hpp>
#include <models/models_manager.hpp>
#include <models/account.hpp>

transaction::transaction(std::shared_ptr<account> from_account_id, 
                         std::shared_ptr<account> to_account_id, 
                         std::shared_ptr<account> category_id, 
                         std::string description, 
                         std::shared_ptr<account> moved_from_account_id, 
                         wxDateTime date, 
                         double amount)
    : _from_account_id(from_account_id), 
      _to_account_id(to_account_id), 
      _category_id(category_id), 
      _description(description), 
      _moved_from_account_id(moved_from_account_id), 
      _date(date), 
      _amount(amount) {}

json transaction::to_json() const {
    json j;
    j["from_account_id"] = _from_account_id ? _from_account_id->generateID() : "";
    j["to_account_id"] = _to_account_id ? _to_account_id->generateID() : "";
    j["category_id"] = _category_id ? _category_id->generateID() : "";
    j["description"] = _description;
    j["moved_from_account_id"] = _moved_from_account_id ? _moved_from_account_id->generateID() : "";
    j["date"] = _date.FormatISODate();
    j["amount"] = _amount;
    return j;
}

void transaction::from_json(const json& j) {
    model_manager& model_manager_ = model_manager::getInstance();
    auto id = j["from_account_id"].get<std::string>();
    if(id != "") {
        _from_account_id = model_manager_.getAccounts()[id];
    }
    
    id = j["to_account_id"].get<std::string>();
    if(id != "") {
        _to_account_id = model_manager_.getAccounts()[id];
    }
    
    id = j["category_id"].get<std::string>();
    if(id != "") {
        _category_id = model_manager_.getAccounts()[id];
    }
    
    _description = j["description"].get<std::string>();

    id = j["moved_from_account_id"].get<std::string>();
    if(id != "") {
        _moved_from_account_id = model_manager_.getAccounts()[id];
    }
    
    _date = wxDateTime();
    if (j.contains("date")) {
        wxString date_str = j["date"].get<std::string>();
        if (!_date.ParseISODate(date_str)) {
            std::cerr << "Error: Invalid date format -> " << std::string(date_str.mb_str()) << std::endl;
        }
    } else {
        std::cerr << "Error: Missing 'date' field in transaction JSON." << std::endl;
    }
    _amount = j["amount"].get<double>();
}

std::string transaction::to_str() {
    std::string from_account_id = _from_account_id ? _from_account_id->generateID() : "";
    std::string to_account_id = _to_account_id ? _to_account_id->generateID() : "";
    std::string category_id = _category_id ? _category_id->generateID() : "";
    std::string moved_from_account_id = _moved_from_account_id ? _moved_from_account_id->generateID() : "";
    return "Transaction: " + from_account_id + " -> " + to_account_id + " ($" + std::to_string(_amount) + ")";
}

// Getters
std::shared_ptr<account> transaction::getFromAccount() const { return _from_account_id; }
std::shared_ptr<account> transaction::getToAccount() const { return _to_account_id; }
std::shared_ptr<account> transaction::getCategory() const { return _category_id; }
std::string transaction::getDescription() const { return _description; }
std::shared_ptr<account> transaction::getMovedFromAccount() const { return _moved_from_account_id; }
wxDateTime transaction::getDate() const { return _date; }
double transaction::getAmount() const { return _amount; }

// Setters
void transaction::setFromAccount(std::shared_ptr<account> from_account_id) { _from_account_id = from_account_id; }
void transaction::setToAccount(std::shared_ptr<account> to_account_id) { _to_account_id = to_account_id; }
void transaction::setCategory(std::shared_ptr<account> category_id) { _category_id = category_id; }
void transaction::setDescription(std::string description) { _description = description; }
void transaction::setMovedFromAccount(std::shared_ptr<account> moved_from_account_id) { _moved_from_account_id = moved_from_account_id; }
void transaction::setDate(wxDateTime date) { _date = date; }
void transaction::setAmount(double amount) { _amount = amount; }
