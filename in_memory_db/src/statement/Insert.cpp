//src/statement/Insert.cpp
#include "statement/Insert.hpp"
#include "db/Database.hpp"
#include <stdexcept>

namespace statement {

Insert::Insert(std::string table_name,
               std::vector<std::unordered_map<std::string, db::Value>> rows)
    : table_name_(std::move(table_name)), rows_(std::move(rows)) {}

void Insert::execute(db::Database& db) {
    if (!db.has_table(table_name_)) {
        throw std::runtime_error("Table '" + table_name_ + "' does not exist.");
    }

    // 取得 Table 的引用 (非指標)
    db::Table& table = db.get_table(table_name_);

    for (const auto& row_data : rows_) {
        table.insert(row_data);  // insert 是 Table 的成員函式
    }
}

}  // namespace statement
