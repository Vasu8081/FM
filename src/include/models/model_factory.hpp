#ifndef MODEL_FACTORY_H
#define MODEL_FACTORY_H

#include <models/model.hpp>
#include <memory>
#include <string>
#include <models/bank_account.hpp>
#include <models/borrow_account.hpp>
#include <models/give_account.hpp>
#include <models/credit_card_account.hpp>
#include <models/transaction.hpp>
#include <models/category.hpp>

class model_factory {
public:
    static std::shared_ptr<model> create(const std::string& id) {
        if (id.find("BA.") == 0 || id.find("Bank Account")==0) {
            return std::make_shared<bank_account>();
        }
        if (id.find("BG.") == 0 || id.find("Borrow Account")==0) {
            return std::make_shared<borrow_account>();
        }
        if (id.find("GV.") == 0 || id.find("Give Account")==0) {
            return std::make_shared<give_account>();
        }
        if (id.find("CC.") == 0 || id.find("Credit Card")==0) {
            return std::make_shared<credit_card_account>();
        }
        if(id.find("Transaction")==0){
            return std::make_shared<transaction>();
        }
        if(id.find("CAT.")==0 || id.find("Category")==0){
            return std::make_shared<category>();
        }
        return nullptr;
    }
};

#endif
