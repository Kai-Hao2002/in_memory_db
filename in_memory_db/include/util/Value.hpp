//include/util/Value.hpp
// include/util/Value.hpp
#pragma once
#include <variant>
#include <string>
#include <iostream>

namespace db {

using Value = std::variant<int, std::string>;

// 比較兩個 Value 是否相等
bool operator==(const Value& lhs, const Value& rhs);
bool operator!=(const Value& lhs, const Value& rhs);

// 幫助格式化 Value（可搭配 SELECT 使用）
std::ostream& operator<<(std::ostream& os, const Value& val);

} // namespace util
