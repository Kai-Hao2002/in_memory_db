//include/util/Value.hpp
#pragma once
#include <variant>
#include <string>
#include <iostream>

namespace db {

// 加入 float、bool，日期用 string 表示（你也可以用專門的 Date 類別）
using Value = std::variant<int,double, float, bool, std::string>;

// 比較兩個 Value 是否相等
bool operator==(const Value& lhs, const Value& rhs);
bool operator!=(const Value& lhs, const Value& rhs);

// 格式化 Value 輸出
std::ostream& operator<<(std::ostream& os, const Value& val);

} // namespace db
