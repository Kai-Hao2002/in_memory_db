// src/db/Database.cpp
#include "db/Database.hpp"
#include <stdexcept>

namespace db {

bool Database::has_table(const std::string& name) const {
    return tables.find(name) != tables.end();
}

void Database::create_table(const std::string& name, const std::vector<Column>& columns) {
    if (tables.find(name) != tables.end()) {
        throw std::runtime_error("Table '" + name + "' already exists.");
    }
    tables.emplace(name, Table{name, columns});
}

Table& Database::get_table(const std::string& name) {
    auto it = tables.find(name);
    if (it == tables.end()) {
        throw std::runtime_error("Table '" + name + "' not found.");
    }
    return it->second;
}

const Table& Database::get_table(const std::string& name) const {
    auto it = tables.find(name);
    if (it == tables.end()) {
        throw std::runtime_error("Table '" + name + "' not found.");
    }
    return it->second;
}

} // namespace db
