// src/statement/Update.cpp
#include "statement/Update.hpp"
#include "db/Database.hpp"
#include "util/StringUtil.hpp"
#include <stdexcept>
#include <algorithm>

namespace statement {

Update::Update(const std::string& table_name,
               const std::string& set_column,
               const db::Value& set_value,
               std::shared_ptr<Condition> where_condition)
    : table_name_(db::to_lower(std::move(table_name))),
      set_column_(db::to_lower(set_column)),  // ⭐ 加入轉小寫
      set_value_(set_value),
      where_condition_(std::move(where_condition)) {}


void Update::execute(db::Database& db) {
    if (!db.has_table(table_name_)) {
        throw std::runtime_error("Table '" + table_name_ + "' does not exist.");
    }
    db::Table& table = db.get_table(table_name_);

    for (auto& row : table.get_rows()) {
        // 如果沒有條件或條件成立才更新
        if (!where_condition_ || where_condition_->evaluate(row, table.columns)) {
            // 找到 set_column_ 在 columns 中的 index
            auto it = std::find_if(table.columns.begin(), table.columns.end(),
                                   [&](const db::Column& col) { return col.name == set_column_; });
            if (it == table.columns.end()) {
                throw std::runtime_error("Column '" + set_column_ + "' not found in table");
            }
            size_t idx = std::distance(table.columns.begin(), it);

            // 更新值
            row.values[idx] = set_value_;
        }
    }
}

}  // namespace statement
