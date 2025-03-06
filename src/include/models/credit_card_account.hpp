#ifndef CREDIT_CARD_ACCOUNT_H
#define CREDIT_CARD_ACCOUNT_H

#include <string>
#include <models/account.hpp>
#include <wx/datetime.h>
#include <wx/msgdlg.h>

class CreditCardAccount : public Account {
public:
    CreditCardAccount(){
        _background_color = wxColour(112, 98, 118);
        _foreground_color = wxColour(240, 230, 250);
    }

    double portfolioValue() const override {
        return _limit_left-_credit_limit;
    }

    std::string getID() const override {
        return "CC." + _card_number;
    }

    std::string getType() const override {
        return "Credit Card";
    }

    json toJson() const override {
        json j = {
            {"ID", getID()},
            {"Card Name", _name},
            {"Card Number", _card_number},
            {"CVV", _cvv},
            {"Expiry Date", _expiry_date.FormatISODate()},
            {"Billing Date", _billing_date.FormatISODate()},
            {"Payment Due Date", _payment_due_date.FormatISODate()},
            {"Credit Limit", _credit_limit},
            {"Limit Left", _limit_left}
        };
        return j;
    }

    void fromJson(const json& j) override {
        if(j.contains("Card Name")) _name = j.at("Card Name").get<std::string>();
        if(j.contains("Card Number")) _card_number = j.at("Card Number").get<std::string>();
        if(j.contains("CVV")) _cvv = j.at("CVV").get<std::string>();
        
        if(j.contains("Expiry Date")) {
            wxDateTime temp;
            temp.ParseISODate(j.at("Expiry Date").get<std::string>());
            if (temp.IsValid()) {
                _expiry_date = temp;
            }
        }

        if(j.contains("Billing Date")) {
            wxDateTime temp;
            temp.ParseISODate(j.at("Billing Date").get<std::string>());
            if (temp.IsValid()) {
                _billing_date = temp;
            }
        }

        if(j.contains("Payment Due Date")) {
            wxDateTime temp;
            temp.ParseISODate(j.at("Payment Due Date").get<std::string>());
            if (temp.IsValid()) {
                _payment_due_date = temp;
            }
        }

        if(j.contains("Credit Limit")) {
            _credit_limit = j.at("Credit Limit").get<double>();
            _limit_left = _credit_limit;
        }
    }

    std::unordered_map<std::string, std::string> inputFormFields() const override {
        return {
            {"Card Name", "string"},
            {"Card Number", "string"},
            {"CVV", "string"},
            {"Expiry Date", "date"},
            {"Billing Date", "date"},
            {"Payment Due Date", "date"},
            {"Credit Limit", "double"}
        };
    }

    std::unordered_map<std::string, std::string> displayFormFields() const override {
        return {
            {"Card Name", _name},
            {"Card Number", _card_number},
            {"CVV", _cvv},
            {"Expiry Date", _expiry_date.FormatISODate().ToStdString()},
            {"Billing Date", _billing_date.FormatISODate().ToStdString()},
            {"Payment Due Date", _payment_due_date.FormatISODate().ToStdString()},
            {"Credit Limit", std::to_string(_credit_limit)},
            {"Limit Left", std::to_string(_limit_left)}
        };
    }

    void amountIn(double amount) override {
        _limit_left += amount;
    }

    void amountOut(double amount) override {
        _limit_left -= amount;
    }

private:
    std::string _card_number;
    std::string _cvv;
    wxDateTime _expiry_date;
    wxDateTime _billing_date;
    wxDateTime _payment_due_date;
    double _credit_limit;
    double _limit_left;
};

#endif // CREDIT_CARD_ACCOUNT_H