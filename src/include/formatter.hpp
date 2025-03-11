#ifndef FORMATTER_HPP
#define FORMATTER_HPP

#include <sstream>
#include <locale>
#include <format>
#include <string>

class Formatter
{

public:
    static std::string Amount(double amount)
    {
        return formatWithCommas(amount);
    }

    static std::string MonthlyPaymentDate(wxDateTime billDate)
    {
        return std::format("{}{} of every month", billDate.GetDay(), getOrdinalSuffix(billDate.GetDay()));
    }

    static std::string MonthYear(wxDateTime date)
    {
        return date.Format("%b-%y").ToStdString();
    }

    static std::string DateMonthYear(wxDateTime date)
    {
        return date.FormatISODate().ToStdString();
    }

    static std::string Integer(int value)
    {
        return std::to_string(value);
    }

    static std::string Percentage(double value)
    {
        return std::format("{:.2f}%", value);
    }

private:
    static std::string getOrdinalSuffix(int day)
    {
        if (day >= 11 && day <= 13)
            return "th";
        switch (day % 10)
        {
        case 1:
            return "st";
        case 2:
            return "nd";
        case 3:
            return "rd";
        default:
            return "th";
        }
    }

    static std::string formatWithCommas(double amount)
    {
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(2) << amount;
        std::string str = stream.str();
        if (str.find(".00") != std::string::npos)
        {
            str = str.substr(0, str.find(".00"));
        }

        int n = str.find('.');
        if (n == std::string::npos)
        {
            n = str.length();
        }

        int insertPosition = n - 3;
        while (insertPosition > 0)
        {
            str.insert(insertPosition, ",");
            insertPosition -= 2;
        }

        return str;
    }
};

#endif // FORMATTER_HPP