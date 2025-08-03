// include/db/Row.hpp
#pragma once
#include <vector>
#include <variant>
#include <string>
#include "util/Value.hpp"

namespace db {


struct Row {
    std::vector<Value> values;
};

} // namespace db

