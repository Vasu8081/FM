#ifndef MODEL_MANAGER_HPP
#define MODEL_MANAGER_HPP

#include <vector>
#include <models/account.hpp>
#include <models/model_factory.hpp>
#include <models/transaction.hpp>
#include <app_config.hpp>

class model_manager
{
public:
    model_manager(model_manager const &) = delete;
    void operator=(model_manager const &) = delete;

    static model_manager &getInstance()
    {
        static model_manager instance;
        return instance;
    }

    void save()
    {
        saveAccounts();
        saveTransactions();
        saveCategories();
    }

    std::map<std::string, std::shared_ptr<Account>> getAccounts() const
    {
        return _accounts;
    }

    std::vector<std::shared_ptr<Transaction>> getTransactions() const
    {
        return _transactions;
    }

    std::map<std::string, std::shared_ptr<Category>> getCategories() const
    {
        return _categories;
    }

    void add(std::shared_ptr<Model> model_)
    {
        std::shared_ptr<Account> account_ = std::dynamic_pointer_cast<Account>(model_);
        if (account_)
        {
            _accounts[account_->getID()] = account_;
        }

        std::shared_ptr<Transaction> transaction_ = std::dynamic_pointer_cast<Transaction>(model_);
        if (transaction_)
        {
            _transactions.push_back(transaction_);
            if (transaction_->getFromAccount())
            {
                _accounts[transaction_->getFromAccount()->getID()]->addFromTransaction(transaction_);
            }
            if (transaction_->getToAccount())
            {
                _accounts[transaction_->getToAccount()->getID()]->addToTransaction(transaction_);
            }
            if (transaction_->getCategory())
            {
                _categories[transaction_->getCategory()->getID()]->addTransaction(transaction_);
            }
        }

        std::shared_ptr<Category> category_ = std::dynamic_pointer_cast<Category>(model_);
        if (category_)
        {
            _categories[category_->getID()] = category_;
        }

        save();
    }

    void loadAccounts()
    {
        _accounts = std::map<std::string, std::shared_ptr<Account>>();
        wxString account_file_path = _config.getAccountDefnsFilePath();

        wxFile accounts_file(account_file_path, wxFile::read);
        if (!accounts_file.IsOpened())
        {
            std::cerr << "Error: Failed to open config file: " << account_file_path << std::endl;
            return;
        }

        wxString accounts;
        accounts_file.ReadAll(&accounts);
        accounts_file.Close();

        try
        {
            json j = json::parse(std::string(accounts.mb_str()));
            for (auto &account_json : j)
            {
                std::string account_id = account_json["ID"].get<std::string>();
                std::shared_ptr<Account> acc = std::dynamic_pointer_cast<Account>(model_factory::create(account_id));
                if (acc)
                {
                    acc->fromJson(account_json);
                    _accounts[acc->getID()] = acc;
                }
                else
                {
                    std::cerr << "Error: Unknown Account type for ID: " << account_id << std::endl;
                }
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error parsing JSON: " << e.what() << std::endl;
        }
    }

    void saveAccounts()
    {
        json j = json::array();
        for (auto &account : _accounts)
        {
            j.push_back(account.second->toJson());
        }

        wxString accounts_file_path = _config.getAccountDefnsFilePath();
        wxFile accounts_file(accounts_file_path, wxFile::write);
        if (!accounts_file.IsOpened())
        {
            std::cerr << "Error: Failed to open config file: " << accounts_file_path << std::endl;
            return;
        }

        accounts_file.Write(j.dump(4));
        accounts_file.Close();
    }

    void loadCategories()
    {
        wxString categories_file_path = _config.getCategoriesFilePath();
        wxFile categories_file(categories_file_path, wxFile::read);
        if (!categories_file.IsOpened())
        {
            std::cerr << "Error: Failed to open config file: " << categories_file_path << std::endl;
            return;
        }

        wxString categories;
        categories_file.ReadAll(&categories);
        categories_file.Close();

        try
        {
            json j = json::parse(std::string(categories.mb_str()));
            for (auto &category_json : j)
            {
                std::shared_ptr<Category> category_ = std::dynamic_pointer_cast<Category>(model_factory::create("Category"));
                category_->fromJson(category_json);
                _categories[category_->getID()] = category_;
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error parsing JSON: " << e.what() << std::endl;
        }
    }

    void saveCategories()
    {
        json j = json::array();
        for (auto &category : _categories)
        {
            j.push_back(category.second->toJson());
        }

        wxString categories_file_path = _config.getCategoriesFilePath();
        wxFile categories_file(categories_file_path, wxFile::write);
        if (!categories_file.IsOpened())
        {
            std::cerr << "Error: Failed to open config file: " << categories_file_path << std::endl;
            return;
        }

        categories_file.Write(j.dump(4));
        categories_file.Close();
    }

    void loadTransactions()
    {
        _transactions = std::vector<std::shared_ptr<Transaction>>();
        wxString transactions_file_path = _config.getTransactionsFilePath();

        wxFile transactions_file(transactions_file_path, wxFile::read);
        if (!transactions_file.IsOpened())
        {
            std::cerr << "Error: Failed to open config file: " << transactions_file_path << std::endl;
            return;
        }

        wxString transactions;
        transactions_file.ReadAll(&transactions);
        transactions_file.Close();

        try
        {
            json j = json::parse(std::string(transactions.mb_str()));
            for (auto &transaction_json : j)
            {
                std::shared_ptr<Transaction> transaction_ = std::dynamic_pointer_cast<Transaction>(model_factory::create("Transaction"));
                transaction_->fromJson(transaction_json);
                if (transaction_->getFromAccount())
                {
                    _accounts[transaction_->getFromAccount()->getID()]->addFromTransaction(transaction_);
                }
                if (transaction_->getToAccount())
                {
                    _accounts[transaction_->getToAccount()->getID()]->addToTransaction(transaction_);
                }
                if (transaction_->getCategory())
                {
                    _categories[transaction_->getCategory()->getID()]->addTransaction(transaction_);
                }
                _transactions.push_back(transaction_);
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error parsing JSON: " << e.what() << std::endl;
        }
    }

    void saveTransactions()
    {
        json j = json::array();
        std::sort(_transactions.begin(), _transactions.end(), [](const std::shared_ptr<Transaction> &a, const std::shared_ptr<Transaction> &b) {
            return a->getDate() < b->getDate();
        });
        for (auto &transaction : _transactions)
        {
            j.push_back(transaction->toJson());
        }

        wxString transactions_file_path = _config.getTransactionsFilePath();
        wxFile transactions_file(transactions_file_path, wxFile::write);
        if (!transactions_file.IsOpened())
        {
            std::cerr << "Error: Failed to open config file: " << transactions_file_path << std::endl;
            return;
        }

        transactions_file.Write(j.dump(4));
        transactions_file.Close();
    }

private:
    app_config &_config = app_config::getInstance();
    model_manager() = default;
    ~model_manager() = default;
    std::map<std::string, std::shared_ptr<Account>> _accounts;
    std::map<std::string, std::shared_ptr<Category>> _categories;
    std::vector<std::shared_ptr<Transaction>> _transactions;
};

#endif // MODEL_MANAGER_HPP