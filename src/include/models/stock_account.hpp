#ifndef STOCK_ACCOUNT_H
#define STOCK_ACCOUNT_H

#include <string>
#include <models/account.hpp>
#include <wx/colour.h>
#include <tradeapi/upstox.hpp>
#include <chrono>
#include <json.hpp>
#include <thread>

using json = nlohmann::json;

class StockAccount : public Account
{
public:
    StockAccount()
    {
        _background_color = wxColour(90, 100, 90);
        _foreground_color = wxColour(235, 245, 235);
    }

    double portfolioValue() const override { return _average_bought_price; }

    std::string getID() const override
    {
        return "STOCK." + _isin;
    }

    std::string getType() const override
    {
        return "Stock Account";
    }

    json toJson() const override
    {
        json j = {
            {"ID", getID()},
            {"Stock Name", _name},
            {"Isin", _isin},
            {"Quantity", _quantity},
            {"Exchange Code", _exchange_code},
            {"Average Bought Price", _average_bought_price}};

        return j;
    }

    void fromJson(const json &j) override
    {
        if (j.contains("Stock Name"))
            _name = j.at("Stock Name").get<std::string>();
        if (j.contains("Isin"))
            _isin = j.at("Isin").get<std::string>();
        if (j.contains("Exchange Code"))
            _exchange_code = j.at("Exchange Code").get<std::string>();
        _product = _api.getProduct(_exchange_code + "%7C" + _isin);
        // std::thread t([this](){
        //     while(true) {
        //         _product = _api.getProduct(_exchange_code+"%7C"+_isin);
        //         notifyObservers();
        //         std::this_thread::sleep_for(std::chrono::seconds(20));
        //     }
        // });
        // t.detach();
    }

    std::unordered_map<std::string, std::string> inputFormFields() const override
    {
        return {
            {"Stock Name", "string"},
            {"Isin", "string"},
            {"Exchange Code", "string"}};
    }

    std::unordered_map<std::string, std::string> displayFormFields() const override
    {
        auto curr_price = _product["data"].begin().value()["last_price"].get<double>();
        auto curr_pnl = (_quantity * curr_price) - (_quantity * _average_bought_price);
        return {
            {"header", _name},
            {"Isin", _name},
            {"Quantity", Formatter::Amount(_quantity)},
            {"PnL Made Till Now", Formatter::Amount(_pnl_made)},
            {"Average Bought Price", Formatter::Amount(_average_bought_price)},
            {"Exchange Code", _exchange_code},
            {"Current Price", Formatter::Amount(curr_price)},
            {"Current PnL", Formatter::Amount(curr_pnl)}};
    }

    std::set<std::string> boldFormFields() const override
    {
        return {"header", "Current PnL", "PnL Made Till Now"};
    }

    std::unordered_map<std::string, wxColour> overrideFormColors() const override
    {
        auto curr_price = _product["data"].begin().value()["last_price"].get<double>();
        auto curr_pnl = (_quantity * curr_price) - (_quantity * _average_bought_price);
        if (curr_pnl < 0)
        {
            return {
                {"Current PnL", wxColour(219, 68, 55)},
                {"PnL Made Till Now", wxColour(255, 191, 0)}};
        }
        else
        {
            return {
                {"Current PnL", wxColour(15, 157, 88)},
                {"PnL Made Till Now", wxColour(255, 191, 0)}};
        }
    }

    void amountIn(std::shared_ptr<Transaction> t) override
    {
        auto total_bought_price = _average_bought_price * _quantity + t->getAmount();
        _quantity += t->getQuantity();
        _average_bought_price = total_bought_price / _quantity;
        notifyObservers();
    }

    void amountOut(std::shared_ptr<Transaction> t) override
    {
        _pnl_made = _pnl_made + (t->getAmount() - _average_bought_price * t->getQuantity());
        _quantity -= t->getQuantity();
        notifyObservers();
    }

private:
    Upstox &_api = Upstox::getInstance();
    std::string _isin;
    double _quantity = 0.0;
    std::string _exchange_code;
    double _average_bought_price = 0.0;
    double _pnl_made = 0.0;
    json _product;
};

/* JSON PRODUCT SAMPLE

{
    "status": "success",
    "data": {
        "NSE_EQ:NHPC": {
            "ohlc": {
                "open": 75.0,
                "high": 76.95,
                "low": 74.32,
                "close": 76.58
            },
            "timestamp": "2025-03-11T16:36:26.853+05:30",
            "instrument_token": "NSE_EQ|INE848E01016",
            "symbol": "NHPC",
            "last_price": 76.58,
            "volume": 12555697,
            "average_price": 75.87,
            "oi": 0.0,
            "net_change": 0.78,
            "total_buy_quantity": 0.0,
            "total_sell_quantity": 978.0,
            "lower_circuit_limit": 68.22,
            "upper_circuit_limit": 83.38,
            "last_trade_time": "1741688897000",
            "oi_day_high": 0.0,
            "oi_day_low": 0.0
        }
    }
}

*/

#endif // STOCK_ACCOUNT_H