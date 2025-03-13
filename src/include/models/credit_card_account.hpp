#ifndef CREDIT_CARD_ACCOUNT_H
#define CREDIT_CARD_ACCOUNT_H

#include <string>
#include <models/account.hpp>
#include <wx/datetime.h>
#include <wx/msgdlg.h>

class CreditCardAccount : public Account
{
public:
    CreditCardAccount()
    {
        _background_color = wxColour(112, 98, 118);
        _foreground_color = wxColour(240, 230, 250);
    }

    double portfolioValue() const override
    {
        return _limit_left - _credit_limit;
    }

    std::string getID() const override
    {
        return "CC." + _card_number;
    }

    std::string getType() const override
    {
        return "Credit Card";
    }

    json toJson() const override
    {
        json j = {
            {"ID", getID()},
            {"Card Name", _name},
            {"Card Number", _card_number},
            {"CVV", _cvv},
            {"Expiry Date", _expiry_date.FormatISODate()},
            {"Billing Date", _billing_date.FormatISODate()},
            {"Payment Due Date", _payment_due_date.FormatISODate()},
            {"Credit Limit", _credit_limit},
            {"Limit Left", _limit_left}};
        return j;
    }

    void fromJson(const json &j) override
    {
        wxDateTime curr_date = wxDateTime::Now();

        if (j.contains("Card Name"))
            _name = j.at("Card Name").get<std::string>();
        if (j.contains("Card Number"))
            _card_number = j.at("Card Number").get<std::string>();
        if (j.contains("CVV"))
            _cvv = j.at("CVV").get<std::string>();

        if (j.contains("Expiry Date"))
        {
            wxDateTime temp;
            temp.ParseISODate(j.at("Expiry Date").get<std::string>());
            if (temp.IsValid())
            {
                _expiry_date = temp;
            }
        }

        if (j.contains("Billing Date"))
        {
            wxDateTime temp;
            temp.ParseISODate(j.at("Billing Date").get<std::string>());
            if (temp.IsValid())
            {
                _billing_date = temp;
                _billing_date.SetYear(curr_date.GetYear());
                _billing_date.SetMonth(curr_date.GetMonth());
        
                if (_billing_date > curr_date) {
                    // If billing date is ahead, move to the previous month
                    _billing_date.SetMonth(wxDateTime::Month(curr_date.GetMonth() - 1));
                    if (curr_date.GetMonth() == wxDateTime::Jan) {
                        _billing_date.SetYear(curr_date.GetYear() - 1); // Handle year transition
                    }
                }
            }
        }
        
        if (j.contains("Payment Due Date"))
        {
            wxDateTime temp;
            temp.ParseISODate(j.at("Payment Due Date").get<std::string>());
            if (temp.IsValid())
            {
                _payment_due_date = temp;
                _payment_due_date.SetYear(curr_date.GetYear());
                _payment_due_date.SetMonth(curr_date.GetMonth());
        
                if (_payment_due_date < _billing_date) {
                    // Ensure due date is after billing date
                    _payment_due_date.SetMonth(wxDateTime::Month(_billing_date.GetMonth() + 1));
                    if (_billing_date.GetMonth() == wxDateTime::Dec) {
                        _payment_due_date.SetYear(_billing_date.GetYear() + 1);
                    }
                }
            }
        }
        
        // If current date is AFTER the due date, shift both to last cycle
        if (curr_date > _payment_due_date)
        {
            // Move billing date to previous cycle
            _billing_date.SetMonth(wxDateTime::Month(_billing_date.GetMonth() - 1));
            if (_billing_date.GetMonth() == wxDateTime::Dec) {
                _billing_date.SetYear(_billing_date.GetYear() - 1);
            }
        
            // Move due date to previous cycle
            _payment_due_date.SetMonth(wxDateTime::Month(_billing_date.GetMonth() + 1));
            if (_payment_due_date.GetMonth() == wxDateTime::Dec) {
                _payment_due_date.SetYear(_payment_due_date.GetYear() - 1);
            }
        }
        

        if (j.contains("Credit Limit"))
        {
            _credit_limit = j.at("Credit Limit").get<double>();
            _limit_left = _credit_limit;
        }
    }

    std::vector<std::pair<std::string, std::string>> inputFormFields() const override
    {
        return {
            {"Card Name", "string"},
            {"Card Number", "string"},
            {"CVV", "string"},
            {"Expiry Date", "date"},
            {"Billing Date", "date"},
            {"Payment Due Date", "date"},
            {"Credit Limit", "double"}};
    }

    std::vector<std::pair<std::string, std::string>> displayFormFields() const override
    {
        return {
            {"header", _name},
            {"Card Number", _card_number},
            // {"Security Code", _cvv},
            // {"Expiry Date", Formatter::MonthYear(_expiry_date)},
            {"Statement Generation", Formatter::MonthlyPaymentDate(_billing_date)},
            {"Due Date", Formatter::MonthlyPaymentDate(_payment_due_date)},
            {"Total Credit Limit", Formatter::Amount(_credit_limit)},
            {"Available Credit", Formatter::Amount(_limit_left)},
            {"Credit Used", Formatter::Amount(_credit_limit-_limit_left)},
            {"Due Amount", Formatter::Amount(_due_amount)}};
    }

    std::set<std::string> boldFormFields() const override
    {
        return {"header", "Available Credit"};
    }

    std::unordered_map<std::string, wxColour> overrideFormColors() const override
    {
        return {
            {"Available Credit", wxColour(235, 170, 235)}};
    }

    void amountIn(std::shared_ptr<Transaction> t) override
    {
        wxDateTime transaction_date = t->getDate();
        double amount = t->getAmount();
        _limit_left += amount;
        _due_amount -= amount;
        notifyObservers();
    }
    
    void amountOut(std::shared_ptr<Transaction> t) override
    {
        wxDateTime transaction_date = t->getDate();
        double amount = t->getAmount();
        _limit_left -= amount;

        if(transaction_date <= _billing_date){
            _due_amount += amount;
        }
        notifyObservers();
    }
    

private:
    std::string _card_number;
    std::string _cvv;
    wxDateTime _expiry_date;
    wxDateTime _billing_date;
    wxDateTime _payment_due_date;
    double _credit_limit;
    double _limit_left;

    double _due_amount = 0;
};

#endif // CREDIT_CARD_ACCOUNT_H