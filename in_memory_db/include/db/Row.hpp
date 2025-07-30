// include/db/Row.hpp
#pragma once
#include <vector>
#include <variant>
#include <string>

namespace db {

using Value = std::variant<int, std::string>;

struct Row {
    std::vector<Value> values;
};

} // namespace db

