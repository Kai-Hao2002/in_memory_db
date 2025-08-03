//src/statement/Insert.cpp
#include "statement/Insert.hpp"
#include "db/Database.hpp"
#include <stdexcept>
#include "util/StringUtil.hpp"

namespace statement {

Insert::Insert(std::string table_name,
               std::vector<std::unordered_map<std::string, db::Value>> rows)
    : table_name_(db::to_lower(std::move(table_name))) {

    // 將欄位名稱一併轉為小寫
    for (const auto& row : rows) {
        std::unordered_map<std::string, db::Value> lowered_row;
        for (const auto& [key, val] : row) {
            lowered_row[db::to_lower(key)] = val;
        }
        rows_.push_back(std::move(lowered_row));
    }
}

void Insert::execute(db::Database& db) {
    if (!db.has_table(table_name_)) {
        throw std::runtime_error("Table '" + table_name_ + "' does not exist.");
    }

    db::Table& table = db.get_table(table_name_);

    for (const auto& row_data : rows_) {
        table.insert(row_data);
    }
}

}  // namespace statement
