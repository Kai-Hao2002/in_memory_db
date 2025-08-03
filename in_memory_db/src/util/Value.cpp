//src/util/Value.cpp
#include "util/Value.hpp"

namespace db {

bool operator==(const Value& lhs, const Value& rhs) {
    return lhs == rhs;  // std::variant 已有 operator== 支援
}

bool operator!=(const Value& lhs, const Value& rhs) {
    return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const Value& val) {
    std::visit([&os](auto&& arg){
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, bool>) {
            os << (arg ? "TRUE" : "FALSE");
        } else {
            os << arg;
        }
    }, val);
    return os;
}

} // namespace db
