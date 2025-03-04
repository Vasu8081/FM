#ifndef MODEL_VIEW_FACTORY_H
#define MODEL_VIEW_FACTORY_H

#include <ui/model_view_panels/model_view.hpp>
#include <ui/model_view_panels/bank_account_view.hpp>
#include <ui/model_view_panels/borrow_account_view.hpp>
#include <ui/model_view_panels/give_account_view.hpp>
#include <ui/model_view_panels/credit_card_account_view.hpp>
#include <ui/model_view_panels/category_view.hpp>
#include <memory>

class model_view_factory {
public:
    static ModelView* create(wxWindow *parent, std::shared_ptr<Model> model) {
        if(auto mdl = std::dynamic_pointer_cast<BankAccount>(model)) {
            return new BankAccountView(parent, mdl);
        }
        if(auto mdl = std::dynamic_pointer_cast<BorrowAccount>(model)) {
            return new BorrowAccountView(parent, mdl);
        }
        if(auto mdl = std::dynamic_pointer_cast<GiveAccount>(model)) {
            return new GiveAccountView(parent, mdl);
        }
        if(auto mdl = std::dynamic_pointer_cast<CreditCardAccount>(model)) {
            return new CreditCardAccountView(parent, mdl);
        }
        if(auto mdl = std::dynamic_pointer_cast<Category>(model)) {
            return new CategoryView(parent, mdl);
        }
        return nullptr;
    }
};

#endif
