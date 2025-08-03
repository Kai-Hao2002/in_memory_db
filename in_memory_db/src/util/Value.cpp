//src/util/Value.cpp
// src/util/Value.cpp
#include "util/Value.hpp"

namespace db {

bool operator==(const Value& lhs, const Value& rhs) {
    return lhs == rhs;
}

bool operator!=(const Value& lhs, const Value& rhs) {
    return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const Value& val) {
    std::visit([&](auto&& v) { os << v; }, val);
    return os;
}

} // namespace util

