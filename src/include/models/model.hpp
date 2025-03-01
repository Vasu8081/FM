#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <json.hpp>

using json = nlohmann::json;

class model {
public:
    model() = default;

    virtual json to_json() = 0;
    virtual void from_json(const json& j) = 0;
};

#endif // MODEL_H