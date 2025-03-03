#ifndef CATEGORY_HPP
#define CATEGORY_HPP

#include <models/model.hpp>
#include <models/transaction.hpp>
#include <vector>
#include <memory>
#include <unordered_map>
#include <json.hpp>

class category : public model {
public:
    category() {
        spends.assign(12, 0.0);
    }

    category(std::string id, std::string name, std::string description, double monthly_budget) 
        : _id(id), _name(name), _description(description), _monthly_budget(monthly_budget) {
        spends.assign(12, 0.0);
    }
    virtual ~category() = default;

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
        if(j.contains("Spends")) spends = j.at("Spends").get<std::vector<double>>();
        if(_id.empty()) _id = generateID();
    }

    std::string toStr() override {
        return "ID: " + _id + ", Name: " + _name + ", Description: " + _description + ", Monthly Budget: " + std::to_string(_monthly_budget);
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
    double getSpentThisMonth() const { return spends[getCurrentMonthIndex()]; }
    double getSpentLastMonth() const { return spends[getPreviousMonthIndex()]; }
    double getAverageThreeMonths() const { return calculateAverage(3); }
    double getAverageSixMonths() const { return calculateAverage(6); }
    double getAverageThisYear() const { return calculateAverage(12); }
    
    // Setters
    void setMonthlyBudget(double monthly_budget) { _monthly_budget = monthly_budget; }

    void addTransaction(std::shared_ptr<transaction> transaction) { 
        _transactions.push_back(transaction);
        double amount = transaction->getAmount();
        wxDateTime transaction_date = transaction->getDate();
        int monthIndex = getMonthIndex(transaction_date);
        
        if (monthIndex >= 0 && monthIndex < 12) {
            spends[monthIndex] += amount;
        }
    }

private:
    std::string _id;
    std::string _name;
    std::string _description;
    double _monthly_budget = 0.0;
    std::vector<double> spends;
    std::vector<std::shared_ptr<transaction>> _transactions;

    int getCurrentMonthIndex() const {
        return wxDateTime::Now().GetMonth();
    }

    int getPreviousMonthIndex() const {
        int current = getCurrentMonthIndex();
        return (current == 0) ? 11 : current - 1;
    }

    int getMonthIndex(const wxDateTime& date) const {
        return date.GetMonth();
    }

    double calculateAverage(int months) const {
        double sum = 0.0;
        int count = std::min(months, 12);
        for (int i = 0; i < count; ++i) {
            sum += spends[getCurrentMonthIndex() - i];
        }
        return count > 0 ? sum / count : 0.0;
    }
};

#endif //CATEGORY_HPP
