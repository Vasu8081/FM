#ifndef ACCOUNT_FACTORY_H
#define ACCOUNT_FACTORY_H

#include <models/account.hpp>
#include <models/bank_account.hpp>
#include <memory>
#include <string>

class account_factory {
public:
    static std::shared_ptr<account> create_account(const std::string& id) {
        if (id.find("BA.") == 0) {  // Bank Account
            return std::make_shared<bank_account>();
        }
        return nullptr;  // Unknown type
    }
};

#endif
