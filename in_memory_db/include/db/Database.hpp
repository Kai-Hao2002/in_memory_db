// include/db/Database.hpp
#pragma once

#include <string>
#include <unordered_map>
#include <vector> 

#include "Table.hpp"

namespace db {

class Database {
public:
    bool has_table(const std::string& name) const;
    void create_table(const std::string& name, const std::vector<Column>& columns);
    Table& get_table(const std::string& name);
    const Table& get_table(const std::string& name) const;

private:
    std::unordered_map<std::string, Table> tables;
};

} // namespace db
