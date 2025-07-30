//src/statement/Select.cpp
#include "statement/Select.hpp"
#include "db/Database.hpp"
#include <stdexcept>
#include <iostream>

namespace statement {

Select::Select(const std::string& table_name,
               const std::vector<std::string>& columns,
               const std::optional<std::pair<std::string, db::Value>>& where)
    : table_name_(table_name), columns_(columns), where_(where) {}

void Select::execute(db::Database& db) {
    if (!db.has_table(table_name_)) {
        throw std::runtime_error("Table '" + table_name_ + "' does not exist.");
    }

    auto& table = db.get_table(table_name_);

    std::vector<std::string> actual_columns;
    if (columns_.size() == 1 && columns_[0] == "*") {
        actual_columns = table.get_column_names();  // 取得所有欄位名的方法，需在 table 實作
    } else {
        actual_columns = columns_;
    }

    auto rows = table.select(actual_columns, where_);

    // 輸出結果
    for (const auto& row : rows) {
        for (const auto& val : row.values) {
            std::visit([](auto&& arg) { std::cout << arg << " "; }, val);
        }
        std::cout << "\n";
    }
}


}  // namespace statement
