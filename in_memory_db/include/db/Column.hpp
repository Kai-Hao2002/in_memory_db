// include/db/Column.hpp
#pragma once
#include <string>


namespace db {

enum class ColumnType { INT, STRING };

struct Column {
    std::string name;
    ColumnType type;
    Column(const std::string& n, ColumnType t) : name(n), type(t) {}
};

} // namespace db
