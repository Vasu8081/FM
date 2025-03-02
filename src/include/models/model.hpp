#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <json.hpp>

using json = nlohmann::json;

class model {
public:
    model() = default;

    virtual json toJson() const = 0;
    virtual void fromJson(const json& j) = 0;

    virtual std::unordered_map<std::string, std::string> fieldTypes() const = 0;

    virtual std::string toStr() = 0;
};

#endif // MODEL_H