#ifndef MODEL_FACTORY_H
#define MODEL_FACTORY_H

#include <models/model.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <models/bank_account.hpp>
#include <models/borrow_account.hpp>
#include <models/give_account.hpp>
#include <models/credit_card_account.hpp>
#include <models/chit_account.hpp>
#include <models/rd_account.hpp>
#include <models/fd_account.hpp>
#include <models/stock_account.hpp>
#include <models/transaction.hpp>
#include <models/category.hpp>

class model_factory
{
public:
    using FactoryFunction = std::function<std::shared_ptr<Model>()>;
    
    static std::shared_ptr<Model> create(const std::string &id)
    {
        for (const auto &[key, creator] : getFactoryMap())
        {
            if (id.find(key) == 0)
            {
                return creator();
            }
        }
        return nullptr;
    }

    static std::vector<std::string> getAccountNames()
    {
        std::vector<std::string> names;
        for (const auto &[key, _] : getFactoryMap())
        {
            if (key.find('.') == std::string::npos && key != "Category" && key != "Transaction")
            {
                names.push_back(key);
            }
        }
        return names;
    }

private:
    static const std::unordered_map<std::string, FactoryFunction> &getFactoryMap()
    {
        static const std::unordered_map<std::string, FactoryFunction> factoryMap = {
            {"BA.", []() { return std::make_shared<BankAccount>(); }},
            {"Bank Account", []() { return std::make_shared<BankAccount>(); }},
            {"BG.", []() { return std::make_shared<BorrowAccount>(); }},
            {"Borrow Account", []() { return std::make_shared<BorrowAccount>(); }},
            {"GV.", []() { return std::make_shared<GiveAccount>(); }},
            {"Give Account", []() { return std::make_shared<GiveAccount>(); }},
            {"CC.", []() { return std::make_shared<CreditCardAccount>(); }},
            {"Credit Card", []() { return std::make_shared<CreditCardAccount>(); }},
            {"CHIT.", []() { return std::make_shared<ChitAccount>(); }},
            {"Chit Account", []() { return std::make_shared<ChitAccount>(); }},
            {"RD.", []() { return std::make_shared<RdAccount>(); }},
            {"RD Account", []() { return std::make_shared<RdAccount>(); }},
            {"STOCK.", []() { return std::make_shared<StockAccount>(); }},
            {"Stock Account", []() { return std::make_shared<StockAccount>(); }},
            {"Transaction", []() { return std::make_shared<Transaction>(); }},
            {"CAT.", []() { return std::make_shared<Category>(); }},
            {"Category", []() { return std::make_shared<Category>(); }},
            {"FD.", []() { return std::make_shared<FdAccount>(); }},
            {"FD Account", []() { return std::make_shared<FdAccount>(); }},
        };
        return factoryMap;
    }
};

#endif