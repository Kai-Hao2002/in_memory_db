//src/statement/Select.cpp
#include "statement/Select.hpp"
#include "db/Database.hpp"
#include <stdexcept>
#include "util/StringUtil.hpp"

namespace statement {

Select::Select(const std::string& table_name,
               const std::vector<std::string>& columns,
               std::shared_ptr<Condition> where,
               bool select_all)
    : table_name_(db::to_lower(table_name)),
      where_(where),
      select_all_(select_all) {

    columns_.reserve(columns.size());
    for (const auto& col : columns) {
        columns_.push_back(db::to_lower(col));
    }
}


void Select::execute(db::Database& db) {
    if (!db.has_table(table_name_)) {
        throw std::runtime_error("Table '" + table_name_ + "' does not exist.");
    }
    auto& table = db.get_table(table_name_);

    std::vector<size_t> col_indexes;
    std::vector<std::string> selected_col_names;

    if (select_all_ || (columns_.size() == 1 && columns_[0] == "*")) {
        for (size_t i = 0; i < table.columns.size(); ++i) {
            col_indexes.push_back(i);
            selected_col_names.push_back(table.columns[i].name);
        }
    } else {
        for (const auto& col_name : columns_) {
            // Process table.column syntax: if there is a ".", only take the column name after the "."
            std::string actual_col_name = col_name;
            size_t dot_pos = col_name.find('.');
            if (dot_pos != std::string::npos) {
                actual_col_name = col_name.substr(dot_pos + 1);
            }

            bool found = false;
            for (size_t i = 0; i < table.columns.size(); ++i) {
                if (table.columns[i].name == actual_col_name) {
                    col_indexes.push_back(i);
                    selected_col_names.push_back(col_name);  // Display still uses the original name
                    found = true;
                    break;
                }
            }
            if (!found)
                throw std::runtime_error("Column '" + col_name + "' not found in table");
        }
    }

    results_.clear();
    output_column_names_.clear();
    output_column_names_ = std::move(selected_col_names);

    for (const auto& row : table.get_rows()) {
        if (!where_ || where_->evaluate(row, table.columns)) {
            std::vector<db::Value> selected_values;
            for (size_t idx : col_indexes) {
                selected_values.push_back(row.values[idx]);
            }
            results_.push_back(std::move(selected_values));
        }
    }
}



} // namespace statement
