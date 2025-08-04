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
    // more types...
};

struct Column {
    std::string name;
    ColumnType type;
    Column(const std::string& n, ColumnType t)
        : name(to_lower(n)),
          type(t) {}
};

} // namespace db
