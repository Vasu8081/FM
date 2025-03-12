#ifndef MODEL_FACTORY_H
#define MODEL_FACTORY_H

#include <models/model.hpp>
#include <memory>
#include <string>
#include <models/bank_account.hpp>
#include <models/borrow_account.hpp>
#include <models/give_account.hpp>
#include <models/credit_card_account.hpp>
#include <models/chit_account.hpp>
#include <models/rd_account.hpp>
#include <models/stock_account.hpp>
#include <models/transaction.hpp>
#include <models/category.hpp>

class model_factory {
    public:
        static model_factory& getInstance() {
            static model_factory instance;
            return instance;
        }
    
        std::shared_ptr<Model> create(const std::string &id) {
            for (const auto& pair : _id_to_class) {
                if (id.find(pair.first) == 0) {
                    return pair.second();
                }
            }

            for(const auto& pair : _name_to_class) {
                if (id.find(pair.first) == 0) {
                    return pair.second();
                }
            }
            
            return nullptr;
        }
    
    private:
        std::unordered_map<std::string, std::function<std::shared_ptr<Model>()>> _id_to_class;
        std::unordered_map<std::string, std::function<std::shared_ptr<Model>()>> _name_to_class;
        model_factory() {
            _id_to_class = {
                {"BA.", []() { return std::make_shared<BankAccount>(); }},
                {"BG.", []() { return std::make_shared<BorrowAccount>(); }},
                {"GV.", []() { return std::make_shared<GiveAccount>(); }},
                {"CC.", []() { return std::make_shared<CreditCardAccount>(); }},
                {"CHIT.", []() { return std::make_shared<ChitAccount>(); }},
                {"RD.", []() { return std::make_shared<RdAccount>(); }},
                {"STOCK.", []() { return std::make_shared<StockAccount>(); }},
                {"CAT.", []() { return std::make_shared<Category>(); }},
            };

            _name_to_class = {
                {"Bank Account", []() { return std::make_shared<BankAccount>(); }},
                {"Borrow Account", []() { return std::make_shared<BorrowAccount>(); }},
                {"Give Account", []() { return std::make_shared<GiveAccount>(); }},
                {"Credit Card", []() { return std::make_shared<CreditCardAccount>(); }},
                {"Chit Account", []() { return std::make_shared<ChitAccount>(); }},
                {"RD Account", []() { return std::make_shared<RdAccount>(); }},
                {"Stock Account", []() { return std::make_shared<StockAccount>(); }},
                {"Transaction", []() { return std::make_shared<Transaction>(); }},
                {"Category", []() { return std::make_shared<Category>(); }},
            };
        }
    };

#endif
