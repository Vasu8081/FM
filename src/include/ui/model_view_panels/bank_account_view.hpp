#ifndef BANK_ACCOUNT_VIEW_HPP
#define BANK_ACCOUNT_VIEW_HPP

#include <ui/model_view_panels/account_view.hpp>
#include <models/bank_account.hpp>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/choice.h>
#include <wx/stattext.h>
#include <wx/statbmp.h>
#include <wx/artprov.h>
#include <wx/dialog.h>

class BankAccountView : public AccountView
{
public:
    BankAccountView(wxWindow *parent, std::shared_ptr<BankAccount> model) : AccountView(parent, model)
    {

    }
};

#endif // BANK_ACCOUNT_VIEW_HPP