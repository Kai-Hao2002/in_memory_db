//src/statement/Delete.cpp
#include "statement/Delete.hpp"
#include "db/Database.hpp"
#include <stdexcept>

namespace statement {

Delete::Delete(std::string table_name,
               std::optional<std::pair<std::string, db::Value>> where)
    : table_name_(std::move(table_name)), where_(std::move(where)) {}

void Delete::execute(db::Database& db) {
    if (!db.has_table(table_name_)) {
        throw std::runtime_error("Table '" + table_name_ + "' does not exist.");
    }

    // 取得 Table 物件（假設是引用）
    db::Table& table = db.get_table(table_name_);

    if (!where_.has_value()) {
        table.delete_all();
    } else {
        const auto& [column, value] = *where_;
        table.delete_where(column, value);
    }
}

}  // namespace statement
