#ifndef MODEL_FACTORY_H
#define MODEL_FACTORY_H

#include <models/model.hpp>
#include <memory>
#include <string>
#include <models/bank_account.hpp>
#include <models/transaction.hpp>
#include <models/category.hpp>

class model_factory {
public:
    static std::shared_ptr<model> create(const std::string& id) {
        if (id.find("BA.") == 0 || id.find("Bank Account")==0) {
            return std::make_shared<bank_account>();
        }
        else if(id.find("Transaction")==0){
            return std::make_shared<transaction>();
        }
        else if(id.find("CAT.")==0 || id.find("Category")==0){
            return std::make_shared<category>();
        }
        return nullptr;
    }
};

#endif
