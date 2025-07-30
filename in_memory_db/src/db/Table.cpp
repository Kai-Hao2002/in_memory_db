// src/db/Table.cpp
#include "db/Table.hpp"
#include <stdexcept>
#include <algorithm>
#include <iterator>

namespace db {

Table::Table(const std::string& name, const std::vector<Column>& columns)
    : name(name), columns(columns) {}

void Table::insert(const std::unordered_map<std::string, Value>& data) {
    Row row;
    for (const auto& col : columns) {
        auto it = data.find(col.name);
        if (it != data.end()) {
            row.values.push_back(it->second);
        } else {
            // Default values
            row.values.push_back(col.type == ColumnType::INT ? Value(0) : Value(""));
        }
    }
    rows.push_back(std::move(row));
}

void Table::delete_all() {
    rows.clear();
}

void Table::delete_where(const std::string& column, const Value& value) {
    auto col_it = std::find_if(columns.begin(), columns.end(),
        [&](const Column& c) { return c.name == column; });

    if (col_it == columns.end()) {
        throw std::runtime_error("Column '" + column + "' not found in DELETE.");
    }

    size_t col_index = std::distance(columns.begin(), col_it);

    rows.erase(std::remove_if(rows.begin(), rows.end(),
        [&](const Row& row) {
            return row.values[col_index] == value;
        }), rows.end());
}

void Table::update_where(const std::string& where_column, const Value& where_value,
                         const std::string& set_column, const Value& set_value) {
    size_t where_idx = columns.size();
    size_t set_idx = columns.size();

    for (size_t i = 0; i < columns.size(); ++i) {
        if (columns[i].name == where_column) where_idx = i;
        if (columns[i].name == set_column) set_idx = i;
    }

    if (where_idx == columns.size() || set_idx == columns.size()) {
        throw std::runtime_error("Column not found in UPDATE.");
    }

    for (auto& row : rows) {
        if (row.values[where_idx] == where_value) {
            row.values[set_idx] = set_value;
        }
    }
}

std::vector<Row> Table::select(const std::vector<std::string>& col_names,
                               const std::optional<std::pair<std::string, Value>>& where) const {
    std::vector<size_t> selected_indices;

    for (const auto& name : col_names) {
        auto it = std::find_if(columns.begin(), columns.end(),
            [&](const Column& c) { return c.name == name; });

        if (it == columns.end()) {
            throw std::runtime_error("Column '" + name + "' not found in SELECT.");
        }

        selected_indices.push_back(std::distance(columns.begin(), it));
    }

    std::vector<Row> result;

    for (const auto& row : rows) {
        if (where) {
            const auto& [where_col, where_val] = *where;
            auto where_it = std::find_if(columns.begin(), columns.end(),
                [&](const Column& c) { return c.name == where_col; });

            if (where_it == columns.end()) {
                throw std::runtime_error("WHERE column '" + where_col + "' not found.");
            }

            size_t where_idx = std::distance(columns.begin(), where_it);
            if (row.values[where_idx] != where_val) {
                continue;
            }
        }

        Row new_row;
        for (size_t idx : selected_indices) {
            new_row.values.push_back(row.values[idx]);
        }
        result.push_back(std::move(new_row));
    }

    return result;
}

std::vector<Row>& Table::get_rows() {
    return rows;
}

const std::vector<Row>& Table::get_rows() const {
    return rows;
}

} // namespace db
