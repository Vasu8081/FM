#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include <sstream>
#include <locale>
#include <format>
#include <string>
#include <wx/datetime.h>

class Calculator
{
public:
    static double findRDInterestRate(double monthly_deposit, double maturity_amount, int tenure, double tol = 1e-6) {
        double low = 0.0, high = 1.0;
        double mid;

        while ((high - low) > tol) {
            mid = (low + high) / 2.0;
            double computedA = calculateMaturityAmount(monthly_deposit, mid, tenure);
            
            if (computedA < maturity_amount)
                low = mid;
            else
                high = mid;
        }
        return mid * 100;
    }

    static int calculateMonthsDifference(wxDateTime start_date, wxDateTime end_date){
        return (end_date.GetYear() - start_date.GetYear()) * 12 + (end_date.GetMonth() - start_date.GetMonth());
    }

    static double calculateInterestAccrued(double principal_remaining, double annual_interest_rate, wxDateTime emi_date, wxDateTime transaction_date, wxDateTime start_date) {
        double days_in_year = transaction_date.IsLeapYear(transaction_date.GetYear()) ? 366.0 : 365.0;
        emi_date.SetYear(transaction_date.GetYear());
        emi_date.SetMonth(transaction_date.GetMonth());
        while (emi_date >= transaction_date) {
            emi_date.Subtract(wxDateSpan::Months(1));
        }
        if (emi_date < start_date) {
            emi_date = start_date;
        }
        int days_between = transaction_date.Subtract(emi_date).GetDays();
        double daily_interest_rate = (annual_interest_rate / 100.0) / days_in_year;
        double interest = principal_remaining * daily_interest_rate * days_between;
    
        return interest;
    }

private:
    static double calculateMaturityAmount(double monthly_deposit, double r, int tenure) {
        double maturity_amount = 0;
        for (int i = 1; i <= tenure; i++) {
            maturity_amount += monthly_deposit * pow((1 + r / 12), (tenure - i + 1));
        }
        return maturity_amount;
    }
};

#endif // CALCULATOR_HPP