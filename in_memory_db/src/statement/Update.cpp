// src/statement/Update.cpp
#include "statement/Update.hpp"
#include "db/Database.hpp"
#include <stdexcept>

namespace statement {

Update::Update(const std::string& table_name,
               const std::string& set_column,
               const db::Value& set_value,
               const std::string& where_column,
               const db::Value& where_value)
    : table_name_(table_name),
      set_column_(set_column),
      set_value_(set_value),
      where_column_(where_column),
      where_value_(where_value) {}

void Update::execute(db::Database& db) {
    // 取得 Table 參考
    db::Table& table = db.get_table(table_name_);
    // 直接呼叫 Table 的 update_where
    table.update_where(where_column_, where_value_, set_column_, set_value_);
}

}  // namespace statement
