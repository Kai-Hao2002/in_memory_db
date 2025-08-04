// src/db/Table.cpp
#include "db/Table.hpp"
#include <stdexcept>
#include <algorithm>
#include <iterator>


namespace db {

Table::Table(const std::string& name, const std::vector<Column>& columns)
    : name(name) {
    this->columns.reserve(columns.size());
    for (const auto& col : columns) {
        this->columns.push_back({db::to_lower(col.name), col.type});
    }
}


void Table::insert(const std::unordered_map<std::string, Value>& data) {
    Row row;
    for (const auto& col : columns) {
        auto it = data.find(col.name);
        if (it != data.end()) {
            if (!value_type_matches_column(it->second, col.type)) {
                throw std::runtime_error("Type mismatch for column '" + col.name + "'");
            }
            row.values.push_back(it->second);
        } else {
            // Infer the default value based on the field type
            switch (col.type) {
                case ColumnType::INT:
                    row.values.push_back(Value(0));
                    break;
                case ColumnType::STRING:
                    row.values.push_back(Value(""));
                    break;
                case ColumnType::FLOAT:
                    row.values.push_back(Value(0.0f));
                    break;
                case ColumnType::DOUBLE:
                    row.values.push_back(Value(0.0));
                    break;
                case ColumnType::BOOL:
                    row.values.push_back(Value(false));
                    break;
                default:
                    throw std::runtime_error("Unsupported column type for default value");
            }
        }
    }
    rows.push_back(std::move(row));
}




void Table::delete_all() {
    rows.clear();
}

void Table::delete_where(std::shared_ptr<statement::Condition> where_condition) {
    if (!where_condition) {
        // Delete all if no conditions are met
        rows.clear();
        return;
    }
    auto it = std::remove_if(rows.begin(), rows.end(), [&](const Row& row) {
        return where_condition->evaluate(row, columns);
    });
    rows.erase(it, rows.end());
}


void Table::update_where(std::shared_ptr<statement::Condition> where_condition,
                        const std::string& set_column, const Value& set_value) {
    // Find the index of set_column
    size_t set_idx = columns.size();
    std::string set_col_lower = db::to_lower(set_column);
    for (size_t i = 0; i < columns.size(); ++i) {
        if (columns[i].name == set_col_lower) {
            set_idx = i;
            break;
        }
    }
    if (set_idx == columns.size()) {
        throw std::runtime_error("Set column not found: " + set_column);
    }


    // If there is no condition, update all
    if (!where_condition) {
        for (auto& row : rows) {
            row.values[set_idx] = set_value;
        }
        return;
    }

    // Update based on the conditions
    for (auto& row : rows) {
        if (where_condition->evaluate(row, columns)) {
            row.values[set_idx] = set_value;
        }
    }
}

std::vector<Row> Table::select(const std::vector<std::string>& cols,
                               std::shared_ptr<statement::Condition> where,
                               bool select_all) const {
    std::vector<std::string> actual_cols;
    if (select_all) {
        actual_cols = get_column_names();
    } else {
        actual_cols = cols;
    }

    std::vector<size_t> indices;
    for (const auto& name : actual_cols) {
        std::string lower_name = db::to_lower(name);
        auto it = std::find_if(columns.begin(), columns.end(),
            [&](const Column& c){ return c.name == lower_name; });
        if (it == columns.end())
            throw std::runtime_error("Column '" + name + "' not found in table");
        indices.push_back(std::distance(columns.begin(), it));
    }

    std::vector<Row> result;
    for (const auto& row : rows) {
        if (!where || where->evaluate(row, columns)) {
            Row nr;
            for (auto idx : indices) nr.values.push_back(row.values[idx]);
            result.push_back(std::move(nr));
        }
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
