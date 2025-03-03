#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <json.hpp>
#include <functional>

using json = nlohmann::json;

class model : public std::enable_shared_from_this<model>
{
public:
    model() = default;

    virtual json toJson() const = 0;
    virtual void fromJson(const json& j) = 0;

    virtual std::unordered_map<std::string, std::string> fieldTypes() const = 0;

    virtual std::string toStr() = 0;

    void registerObserver(std::function<void()> observer)
    {
        _observers.push_back(observer);
    }

private:
    std::vector<std::function<void()>> _observers;

protected:
    void notifyObservers()
    {
        for (auto observer : _observers)
        {
            observer();
        }
    }
};

#endif // MODEL_H