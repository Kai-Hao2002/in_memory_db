// include/db/Column.hpp
#pragma once
#include <string>
#include <stdexcept>
#include "util/StringUtil.hpp"

namespace db {

enum class ColumnType {
    INT,
    STRING,
    FLOAT,
    DOUBLE,
    BOOL,
    DATE,
    // 你可以繼續新增更多類型...
};

struct Column {
    std::string name;
    ColumnType type;
    Column(const std::string& n, ColumnType t)
        : name(to_lower(n)),
          type(t) {}
};

// 轉字串到 ColumnType，方便 Parser 使用
inline ColumnType parse_column_type(const std::string& type_str) {
    std::string t = to_lower(type_str);
    if (t == "int") return ColumnType::INT;
    if (t == "string") return ColumnType::STRING;
    if (t == "double") return ColumnType::DOUBLE;
    if (t == "float") return ColumnType::FLOAT;
    if (t == "bool") return ColumnType::BOOL;
    if (t == "date") return ColumnType::DATE;
    throw std::runtime_error("Unknown column type: " + type_str);
}

} // namespace db
