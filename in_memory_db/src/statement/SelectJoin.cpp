// src/statement/SelectJoin.cpp
#include "statement/SelectJoin.hpp"
#include <stdexcept>
#include "util/StringUtil.hpp"

namespace statement {

SelectJoin::SelectJoin(const std::string& left_table,
                       const std::string& right_table,
                       const std::vector<std::string>& select_columns,
                       const std::string& left_join_column,
                       const std::string& right_join_column,
                       std::shared_ptr<Condition> where,
                       bool select_all)
    : left_table_(db::to_lower(left_table)),
      right_table_(db::to_lower(right_table)),
      where_(where),
      left_join_column_(db::to_lower(left_join_column)),
      right_join_column_(db::to_lower(right_join_column)),
      select_all_(select_all) {
    
    for (const auto& col : select_columns) {
        select_columns_.push_back(db::to_lower(col));
    }
}

void SelectJoin::execute(db::Database& db) {
    if (!db.has_table(left_table_))
        throw std::runtime_error("Table '" + left_table_ + "' not found.");
    if (!db.has_table(right_table_))
        throw std::runtime_error("Table '" + right_table_ + "' not found.");

    const auto& left_tbl = db.get_table(left_table_);
    const auto& right_tbl = db.get_table(right_table_);

    // Convert "table.column" format to "column"
    auto extract_col_name = [](const std::string& full) -> std::string {
        size_t dot = full.find('.');
        return (dot != std::string::npos) ? full.substr(dot + 1) : full;
    };

    std::string left_join_col_name = extract_col_name(left_join_column_);
    std::string right_join_col_name = extract_col_name(right_join_column_);

    // Find the join field index
    size_t left_join_idx = left_tbl.columns.size();
    for (size_t i = 0; i < left_tbl.columns.size(); ++i) {
        if (left_tbl.columns[i].name == left_join_col_name) {
            left_join_idx = i;
            break;
        }
    }
    if (left_join_idx == left_tbl.columns.size())
        throw std::runtime_error("Join column '" + left_join_column_ + "' not found in table '" + left_table_ + "'");

    size_t right_join_idx = right_tbl.columns.size();
    for (size_t i = 0; i < right_tbl.columns.size(); ++i) {
        if (right_tbl.columns[i].name == right_join_col_name) {
            right_join_idx = i;
            break;
        }
    }
    if (right_join_idx == right_tbl.columns.size())
        throw std::runtime_error("Join column '" + right_join_column_ + "' not found in table '" + right_table_ + "'");

    // Collect output field information
    struct ColInfo {
        const db::Table* table;
        size_t col_idx;
    };
    std::vector<ColInfo> selected_col_info;

    results_.clear();
    output_column_names_.clear();

    if (select_all_) {
        select_columns_.clear();  // Automatically generate column names (table.col)

        for (size_t i = 0; i < left_tbl.columns.size(); ++i) {
            std::string col_name = left_table_ + "." + left_tbl.columns[i].name;
            select_columns_.push_back(col_name);
            output_column_names_.push_back(col_name);
            selected_col_info.push_back({ &left_tbl, i });
        }
        for (size_t i = 0; i < right_tbl.columns.size(); ++i) {
            std::string col_name = right_table_ + "." + right_tbl.columns[i].name;
            select_columns_.push_back(col_name);
            output_column_names_.push_back(col_name);
            selected_col_info.push_back({ &right_tbl, i });
        }
    } else {
        for (const auto& full_col_name : select_columns_) {
            auto pos = full_col_name.find('.');
            if (pos == std::string::npos)
                throw std::runtime_error("Select column '" + full_col_name + "' must be qualified with table name.");

            std::string tbl_name = full_col_name.substr(0, pos);
            std::string col_name = full_col_name.substr(pos + 1);

            if (tbl_name != left_table_ && tbl_name != right_table_) {
                throw std::runtime_error("Unknown table '" + tbl_name + "' in select columns.");
            }

            const db::Table* tbl_ptr = (tbl_name == left_table_) ? &left_tbl : &right_tbl;
            size_t col_idx = tbl_ptr->columns.size();
            for (size_t i = 0; i < tbl_ptr->columns.size(); ++i) {
                if (tbl_ptr->columns[i].name == col_name) {
                    col_idx = i;
                    break;
                }
            }
            if (col_idx == tbl_ptr->columns.size())
                throw std::runtime_error("Column '" + col_name + "' not found in table '" + tbl_name + "'.");

            selected_col_info.push_back({ tbl_ptr, col_idx });
            output_column_names_.push_back(full_col_name);
        }
    }

    // Do an INNER JOIN (simple nested loop)
    for (const auto& left_row : left_tbl.get_rows()) {
        for (const auto& right_row : right_tbl.get_rows()) {
            if (left_row.values[left_join_idx] == right_row.values[right_join_idx]) {
                std::vector<db::Value> joined_row;
                for (const auto& col_info : selected_col_info) {
                    if (col_info.table == &left_tbl) {
                        joined_row.push_back(left_row.values[col_info.col_idx]);
                    } else {
                        joined_row.push_back(right_row.values[col_info.col_idx]);
                    }
                }
                results_.push_back(std::move(joined_row));
            }
        }
    }
}


} // namespace statement
