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

class model_factory
{
public:
    static std::shared_ptr<Model> create(const std::string &id)
    {
        if (id.find("BA.") == 0 || id.find("Bank Account") == 0)
        {
            return std::make_shared<BankAccount>();
        }
        if (id.find("BG.") == 0 || id.find("Borrow Account") == 0)
        {
            return std::make_shared<BorrowAccount>();
        }
        if (id.find("GV.") == 0 || id.find("Give Account") == 0)
        {
            return std::make_shared<GiveAccount>();
        }
        if (id.find("CC.") == 0 || id.find("Credit Card") == 0)
        {
            return std::make_shared<CreditCardAccount>();
        }
        if (id.find("CHIT.") == 0 || id.find("Chit Account") == 0)
        {
            return std::make_shared<ChitAccount>();
        }
        if (id.find("RD.") == 0 || id.find("RD Account") == 0)
        {
            return std::make_shared<RdAccount>();
        }
        if (id.find("STOCK.") == 0 || id.find("Stock Account") == 0)
        {
            return std::make_shared<StockAccount>();
        }
        if (id.find("Transaction") == 0)
        {
            return std::make_shared<Transaction>();
        }
        if (id.find("CAT.") == 0 || id.find("Category") == 0)
        {
            return std::make_shared<Category>();
        }
        return nullptr;
    }
};

#endif
