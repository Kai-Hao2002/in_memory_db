// src/db/Database.cpp
#include "db/Database.hpp"
#include "parser/Parser.hpp"
#include "util/StringUtil.hpp"
#include <stdexcept>


namespace db {

bool Database::has_table(const std::string& name) const {
    return tables.find(db::to_lower(name)) != tables.end();
}


void Database::create_table(const std::string& name, const std::vector<Column>& columns) {
    std::string lower_name = db::to_lower(name);
    if (tables.find(lower_name) != tables.end()) {
        throw std::runtime_error("Table '" + name + "' already exists.");
    }

    std::vector<Column> lower_columns;
    for (const auto& col : columns) {
        lower_columns.emplace_back(db::to_lower(col.name), col.type);
    }

    tables.emplace(lower_name, Table{lower_name, lower_columns});
}


Table& Database::get_table(const std::string& name) {
    std::string lower_name = db::to_lower(name);
    auto it = tables.find(lower_name);
    if (it == tables.end()) {
        throw std::runtime_error("Table '" + name + "' not found.");
    }
    return it->second;
}

const Table& Database::get_table(const std::string& name) const {
    std::string lower_name = db::to_lower(name);
    auto it = tables.find(lower_name);
    if (it == tables.end()) {
        throw std::runtime_error("Table '" + name + "' not found.");
    }
    return it->second;
}


void Database::insert(const std::string& table_name, const std::unordered_map<std::string, Value>& data) {
    std::string lower_name = db::to_lower(table_name);
    if (!has_table(lower_name)) {
        throw std::runtime_error("Table '" + table_name + "' does not exist.");
    }
    Table& table = get_table(lower_name);

    // 把 key 都轉成小寫再呼叫 Table::insert
    std::unordered_map<std::string, Value> lower_data;
    for (const auto& [k, v] : data) {
        lower_data.emplace(db::to_lower(k), v);
    }
    table.insert(lower_data);
}



std::vector<Row> Database::execute(const std::string& sql) {
    Parser parser(sql);
    auto stmt = parser.parse();  // statement::StatementPtr

    stmt->execute(*this);  // 多型調用 execute()

    std::vector<Row> result_rows;

    if (stmt->has_results()) {
        for (const auto& values : stmt->get_results()) {
            result_rows.push_back(Row{values});
        }
    }

    return result_rows;
}



} // namespace db
