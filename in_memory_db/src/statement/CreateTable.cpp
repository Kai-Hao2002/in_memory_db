//src/statement/CreateTable.cpp
#include "statement/CreateTable.hpp"
#include "db/Database.hpp"
#include "db/Column.hpp"
#include "util/StringUtil.hpp"
#include <stdexcept>

namespace statement {

CreateTableStatement::CreateTableStatement(std::string table_name, std::vector<db::Column> columns)
    : table_name_(db::to_lower(std::move(table_name))) {
    
    for (auto& col : columns) {
        col.name = db::to_lower(col.name);
    }
    columns_ = std::move(columns);
}

void CreateTableStatement::execute(db::Database& db) {
    if (!db.has_table(table_name_)) {
        db.create_table(table_name_, columns_);
    } else {
        throw std::runtime_error("Table '" + table_name_ + "' already exists.");
    }
}

}  // namespace statement
