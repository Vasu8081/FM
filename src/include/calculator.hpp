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