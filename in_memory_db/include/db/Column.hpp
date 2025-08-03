// include/db/Column.hpp
#pragma once
#include <string>
#include "util/StringUtil.hpp"  // for to_lower()

namespace db {

enum class ColumnType { INT, STRING };

struct Column {
    std::string name;
    ColumnType type;
    Column(const std::string& n, ColumnType t)
        : name(to_lower(n)),  // 這裡轉小寫
          type(t) {}
};

} // namespace db
