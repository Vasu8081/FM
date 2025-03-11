#ifndef MODEL_VIEW_FACTORY_H
#define MODEL_VIEW_FACTORY_H

#include <ui/model_view_panels/model_view.hpp>
#include <ui/model_view_panels/account_view.hpp>
#include <ui/model_view_panels/category_view.hpp>
#include <memory>

class model_view_factory
{
public:
    static ModelView *create(wxWindow *parent, std::shared_ptr<Model> model)
    {
        if (auto mdl = std::dynamic_pointer_cast<Account>(model))
        {
            return new AccountView(parent, mdl);
        }
        if (auto mdl = std::dynamic_pointer_cast<Category>(model))
        {
            return new CategoryView(parent, mdl);
        }
        return nullptr;
    }
};

#endif
