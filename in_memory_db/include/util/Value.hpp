//include/util/Value.hpp
#pragma once
#include <variant>
#include <string>
#include <iostream>

namespace db {

// Add float, bool, and use string to represent dates
using Value = std::variant<int, float, double, bool, std::string>;

// Formatting Value Output
std::ostream& operator<<(std::ostream& os, const Value& val);

} // namespace db
