#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <json.hpp>
#include <functional>

using json = nlohmann::json;

class Model : public std::enable_shared_from_this<Model>
{
public:
    Model() = default;

    virtual std::string getType() const = 0;

    virtual json toJson() const = 0;
    virtual void fromJson(const json &j) = 0;

    virtual std::vector<std::pair<std::string, std::string>> inputFormFields() const = 0;

    void registerObserver(std::function<void()> observer)
    {
        _observers.push_back(observer);
    }

    void notifyObservers()
    {
        for (auto observer : _observers)
        {
            observer();
        }
    }

private:
    std::vector<std::function<void()>> _observers;
};

#endif // MODEL_H