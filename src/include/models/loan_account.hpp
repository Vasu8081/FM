#ifndef LOAN_ACCOUNT_HPP
#define LOAN_ACCOUNT_HPP

#include <models/account.hpp>

class LoanAccount : public Account {
public:
    LoanAccount() {
        _background_color = wxColour(140, 98, 62);
        _foreground_color = wxColour(235, 245, 235);
    }

    std::pair<std::string, double> portfolioValue() const override { return {"Principal Outstanding", _principal_left }; }

    bool isDebtAccount() const override { return true; }

    std::string getID() const override {
        return "LA." + _name;
    }

    std::string getType() const override {
        return "Loan Account";
    }

    json toJson() const override {
        json j = {
            {"ID", getID()},
            {"Loan Name", _name},
            {"Loan Amount", _loan_amount},
            {"Rate of Interest", _roi},
            {"Pre EMI", _pre_emi},
            {"Tenure", _tenure},
            {"Start Date", _start_date.FormatISODate()},
            {"EMI", _emi},
            {"EMI Date", _emi_date.FormatISODate()},
            {"Principal Left", _principal_left},
            {"Interest Paid", _interest_paid}};

        return j;
    }

    void fromJson(const json &j) override {
        if (j.contains("Loan Name"))
            _name = j.at("Loan Name").get<std::string>();
        if (j.contains("Rate of Interest"))
            _roi = j.at("Rate of Interest").get<double>();
        if (j.contains("Pre EMI"))
            _pre_emi = j.at("Pre EMI").get<double>();
        if (j.contains("Tenure"))
            _tenure = j.at("Tenure").get<double>();
        if (j.contains("Start Date"))
            _start_date.ParseISODate(j.at("Start Date").get<std::string>());
        if (j.contains("EMI Date"))
            _emi_date.ParseISODate(j.at("EMI Date").get<std::string>());
        if (j.contains("EMI"))
            _emi = j.at("EMI").get<double>();
    }

    std::vector<std::pair<std::string, std::string>> inputFormFields() const override {
        return {
            {"Loan Name", "string"},
            {"Rate of Interest", "double"},
            {"Pre EMI", "double"},
            {"Tenure", "double"},
            {"Start Date", "date"},
            {"EMI Date", "date"},
            {"EMI", "double"}};
    }

    std::vector<std::pair<std::string, std::string>> displayFormFields() const override {
        return {
            {"header", _name},
            {"Loan Amount", Formatter::Amount(_loan_amount)},
            {"Pre EMI", Formatter::Amount(_pre_emi)},
            {"Rate of Interest", Formatter::Percentage(_roi)},
            {"Tenure", Formatter::Integer(_tenure)},
            {"Start Date", Formatter::DateMonthYear(_start_date)},
            {"EMI", Formatter::Amount(_emi)},
            {"EMI Date", Formatter::MonthlyPaymentDate(_emi_date)},
            {"Principal Left", Formatter::Amount(_principal_left)},
            {"Interest Paid", Formatter::Amount(_interest_paid)}};
    }

    std::set<std::string> boldFormFields() const override
    {
        return {"header", "EMI"};
    }

    std::unordered_map<std::string, wxColour> overrideFormColors() const override
    {
        return {
            {"EMI", wxColour(235, 170, 235)}};
    }

    void amountIn(std::shared_ptr<Transaction> t) override {
        auto interest_rate = _roi;
        if(t->getInterestRate() > 0 ){
            interest_rate = t->getInterestRate();
        }
        auto interest_accrued = Calculator::calculateInterestAccrued(_principal_left, interest_rate, _emi_date, t->getDate(), _start_date);
        _principal_left -= (t->getAmount()-interest_accrued);
        _interest_paid += interest_accrued;
        notifyObservers();
    }

    void amountOut(std::shared_ptr<Transaction> t) override {
        _loan_amount += t->getAmount();
        _principal_left += t->getAmount();
        notifyObservers();
    }

    double getInterestRate() const {
        return _roi;
    }

private:
    double _pre_emi; // how much we paid before the EMI started
    double _loan_amount; // tells total how much loan we took
    double _roi; // rate of interest
    double _tenure; // how many months we have to pay
    wxDateTime _start_date; // when we took the loan
    wxDateTime _emi_cal_date; // when we have to calculate the EMI
    wxDateTime _emi_date; // when we have to pay the EMI
    double _emi; // how much we have to pay every month
    double _principal_left; // how much principal is left
    double _interest_paid; // how much interest we have paid
};

#endif // LOAN_ACCOUNT_HPP