// include/db/Database.hpp
#pragma once

#include <string>
#include <unordered_map>
#include <vector> 
#include "Table.hpp"
#include "parser/Parser.hpp"
#include "statement/Statement.hpp"
#include "statement/CreateTable.hpp"
#include "statement/Insert.hpp"
#include "statement/Select.hpp"
#include "statement/Update.hpp"
#include "statement/Delete.hpp"

namespace db {

class Database {
public:
    bool has_table(const std::string& name) const;
    void create_table(const std::string& name, const std::vector<Column>& columns);
    Table& get_table(const std::string& name);
    const Table& get_table(const std::string& name) const;
    void insert(const std::string& table_name, const std::unordered_map<std::string, db::Value>& data);
    std::vector<Row> execute(const std::string& sql);
    void save_to_file(const std::string& filename) const;
    void load_from_file(const std::string& filename);


private:
    std::unordered_map<std::string, Table> tables;
};

} // namespace db
