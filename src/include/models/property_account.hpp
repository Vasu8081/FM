#ifndef PROPERTY_ACCOUNT_HPP
#define PROPERTY_ACCOUNT_HPP

#include <models/account.hpp>

class PropertyAccount : public Account {
public:
    PropertyAccount() {
        _background_color = wxColour(156, 118, 107);
        _foreground_color = wxColour(235, 245, 235);
    }

    std::pair<std::string, double> portfolioValue() const override { return {"Property Value", _bought_value}; }

    bool isInvestmentAccount() const override { return true; }

    std::string getID() const override {
        return "PA." + _name;
    }

    std::string getType() const override {
        return "Property Account";
    }

    json toJson() const override {
        json j = {
            {"ID", getID()},
            {"Property Name", _name},
            {"Bought Value", _bought_value},
            {"Address", _address},
            {"Money Spent", _money_spent}
        };

        return j;
    }

    void fromJson(const json &j) override {
        if (j.contains("Property Name")) {
            _name = j.at("Property Name").get<std::string>();
        }
        if (j.contains("Bought Value")) {
            _bought_value = j.at("Bought Value").get<double>();
        }
        if (j.contains("Address")) {
            _address = j.at("Address").get<std::string>();
        }
        if (j.contains("Money Spent")) {
            _money_spent = j.at("Money Spent").get<double>();
        }
    }

    std::vector<std::pair<std::string, std::string>> inputFormFields() const override {
        return {
            {"Property Name", "string"},
            {"Bought Value", "double"},
            {"Address", "string"},
            {"Money Spent", "double"}
        };
    }

    std::vector<std::pair<std::string, std::string>> displayFormFields() const override {
        return {
            {"Property Name", _name},
            {"Bought Value", Formatter::Amount(_bought_value)},
            {"Address", _address},
            {"Money Spent", Formatter::Amount(_money_spent)}
        };
    }

    std::set<std::string> boldFormFields() const override {
        return {"Property Name", "Bought Value"};
    }

    std::unordered_map<std::string, wxColour> overrideFormColors() const override {
        return {};
        return {
            {"Property Name", wxColour(235, 245, 235)},
            {"Bought Value", wxColour(235, 245, 235)}
        };
    }

    void amountIn(std::shared_ptr<Transaction> t) override {
        _money_spent += t->getAmount();
        notifyObservers();
    }

    void amountOut(std::shared_ptr<Transaction> t) override {
        _money_spent -= t->getAmount();
        notifyObservers();
    }

private:
    double _bought_value;
    double _money_spent;
    std::string _address;
};

#endif // PROPERTY_ACCOUNT_HPP