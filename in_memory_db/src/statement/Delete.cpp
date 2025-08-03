//src/statement/Delete.cpp
#include "statement/Delete.hpp"
#include "db/Database.hpp"
#include <stdexcept>
#include <algorithm>
#include "util/StringUtil.hpp"

namespace statement {

Delete::Delete(std::string table_name,
               std::shared_ptr<Condition> where_condition)
    : table_name_(db::to_lower(std::move(table_name))), where_condition_(std::move(where_condition)) {}

void Delete::execute(db::Database& db) {
    if (!db.has_table(table_name_)) {
        throw std::runtime_error("Table '" + table_name_ + "' does not exist.");
    }

    db::Table& table = db.get_table(table_name_);

    if (!where_condition_) {
        // 無條件，刪除所有
        table.delete_all();
    } else {
        // 有條件，刪除符合條件的列
        auto& rows = table.get_rows();
        rows.erase(
            std::remove_if(rows.begin(), rows.end(),
                [&](const db::Row& row) {
                    return where_condition_->evaluate(row, table.columns);
                }),
            rows.end()
        );
    }
}

}  // namespace statement
