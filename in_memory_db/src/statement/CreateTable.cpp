//src/statement/CreateTable.cpp
#include "statement/CreateTable.hpp"
#include "db/Database.hpp"
#include "db/Column.hpp"
#include <stdexcept>

namespace statement {

CreateTableStatement::CreateTableStatement(std::string table_name, std::vector<db::Column> columns)
    : table_name_(std::move(table_name)), columns_(std::move(columns)) {}

void CreateTableStatement::execute(db::Database& db) {
    // 下面是針對 Database 的方法檢查與執行
    if (!db.has_table(table_name_)) {
        db.create_table(table_name_, columns_);
    } else {
        throw std::runtime_error("Table '" + table_name_ + "' already exists.");
    }
}

}  // namespace statement
