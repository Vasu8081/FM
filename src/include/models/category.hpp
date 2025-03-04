#ifndef CATEGORY_HPP
#define CATEGORY_HPP

#include <models/model.hpp>
#include <models/transaction.hpp>
#include <vector>
#include <memory>
#include <unordered_map>
#include <json.hpp>

class Category : public Model {
public:
    Category() {
        spends.assign(12, 0.0);
    }

    virtual ~Category() = default;

    std::string generateID() const { return "CAT." + _name; };

    nlohmann::json toJson() const override {
        nlohmann::json j = {
            {"Id", _id},
            {"Name", _name},
            {"Description", _description},
            {"Monthly Budget", _monthly_budget},
            {"Spends", spends}
        };
        return j;
    }

    void fromJson(const nlohmann::json& j) override {
        if(j.contains("Id")) _id = j.at("Id").get<std::string>();
        if(j.contains("Name")) _name = j.at("Name").get<std::string>();
        if(j.contains("Description")) _description = j.at("Description").get<std::string>();
        if(j.contains("Monthly Budget")) _monthly_budget = j.at("Monthly Budget").get<double>();
        if(_id.empty()) _id = generateID();
    }

    std::unordered_map<std::string, std::string> fieldTypes() const override {
        return {
            {"Name", "string"},
            {"Description", "string"},
            {"Monthly Budget", "double"}
        };
    }

    // Getters
    std::string getID() const { return _id; }
    std::string getName() const { return _name; }
    std::string getDescription() const { return _description; }
    double getMonthlyBudget() const { return _monthly_budget; }
    double getSpentThisMonth() const { return spends[0]; }
    double getSpentLastMonth() const { return spends[1]; }
    double getAverageThreeMonths() const { return calculateAverage(3); }
    std::vector<std::shared_ptr<Transaction>> getTransactions() const { return _transactions; }
    
    // Setters
    void setMonthlyBudget(double monthly_budget) { _monthly_budget = monthly_budget; }

    void addTransaction(std::shared_ptr<Transaction> transaction) { 
        _transactions.push_back(transaction);
        double amount = transaction->getAmount();
        wxDateTime transaction_date = transaction->getDate();
        int monthOffset = getMonthOffsetFromDate(transaction_date);
        
        if (monthOffset >= 0 && monthOffset < 12) {
            spends[monthOffset] += amount;
        }
    }

private:
    std::string _id;
    std::string _name;
    std::string _description;
    double _monthly_budget = 0.0;
    std::vector<double> spends;
    std::vector<std::shared_ptr<Transaction>> _transactions;

    int getMonthOffset(int offset) const {
        wxDateTime now = wxDateTime::Now();
        int month = now.GetMonth() - offset;
        int yearOffset = (month < 0) ? 1 : 0;
        month = (month + 12) % 12;
        return month;
    }

    int getMonthOffsetFromDate(const wxDateTime& date) const {
        wxDateTime now = wxDateTime::Now();
        int diffMonths = (now.GetYear() - date.GetYear()) * 12 + (now.GetMonth() - date.GetMonth());
        return (diffMonths >= 0 && diffMonths < 12) ? diffMonths : -1;
    }

    double calculateAverage(int months) const {
        double sum = 0.0;
        int count = std::min(months, 12);
        for (int i = 0; i < count; ++i) {
            sum += spends[getMonthOffset(i)];
        }
        return count > 0 ? sum / count : 0.0;
    }
};

#endif //CATEGORY_HPP
