#ifndef MODEL_MANAGER_HPP
#define MODEL_MANAGER_HPP

#include <vector>
#include <models/account.hpp>
#include <models/account_factory.hpp>
#include <models/transaction.hpp>
#include <app_config.hpp>

class model_manager
{
public:
    model_manager(model_manager const&) = delete;
    void operator=(model_manager const&) = delete;

    static model_manager& getInstance() {
        static model_manager instance;
        return instance;
    }

    std::map<std::string, std::shared_ptr<account>> getAccounts() const {
        return _accounts;
    }

    std::vector<std::shared_ptr<transaction>> getTransactions() const {
        return _transactions;
    }

    void loadAccounts() {
        _accounts = std::map<std::string, std::shared_ptr<account>>();
        wxString account_file_path = _config.getAccountDefnsFilePath();

        wxFile accounts_file(account_file_path, wxFile::read);
        if (!accounts_file.IsOpened()) {
            std::cerr << "Error: Failed to open config file: " << account_file_path << std::endl;
            return;
        }
    
        wxString accounts;
        accounts_file.ReadAll(&accounts);
        accounts_file.Close();

        try {
            json j = json::parse(std::string(accounts.mb_str()));
            for (auto& account_json : j) {
                std::string account_id = account_json["id"].get<std::string>();
                
                std::shared_ptr<account> acc = account_factory::create_account(account_id);
                if (acc) {
                    acc->from_json(account_json);
                    _accounts[acc->generateID()] = acc;
                } else {
                    std::cerr << "Error: Unknown account type for ID: " << account_id << std::endl;
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Error parsing JSON: " << e.what() << std::endl;
        }
    }

    void loadTransactions() {
        _transactions = std::vector<std::shared_ptr<transaction>>();
        wxString transactions_file_path = _config.getTransactionsFilePath();

        wxFile transactions_file(transactions_file_path, wxFile::read);
        if (!transactions_file.IsOpened()) {
            std::cerr << "Error: Failed to open config file: " << transactions_file_path << std::endl;
            return;
        }

        wxString transactions;
        transactions_file.ReadAll(&transactions);
        transactions_file.Close();

        try {
            json j = json::parse(std::string(transactions.mb_str()));
            for (auto& transaction_json : j) {
                std::shared_ptr<transaction> transaction_ = std::make_shared<transaction>();
                transaction_->from_json(transaction_json);
                if(transaction_->getFromAccount()) {
                    _accounts[transaction_->getFromAccount()->generateID()]->addFromTransaction(transaction_);
                }
                if(transaction_->getToAccount()) {
                    _accounts[transaction_->getToAccount()->generateID()]->addToTransaction(transaction_);
                }
                _transactions.push_back(transaction_);
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error parsing JSON: " << e.what() << std::endl;
        }
    }

private:
    app_config& _config = app_config::getInstance();
    model_manager() = default;
    ~model_manager() = default;
    std::map<std::string, std::shared_ptr<account>> _accounts;
    std::vector<std::shared_ptr<transaction>> _transactions;
};

#endif // MODEL_MANAGER_HPP