// src/db/Database.cpp
#include "db/Database.hpp"
#include "parser/Parser.hpp"
#include "util/StringUtil.hpp"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

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

void Database::save_to_file(const std::string& filename) const {
    std::ofstream ofs(filename);
    if (!ofs) throw std::runtime_error("Failed to open file for writing");

    ofs << "#INMEMDB_V1\n";

    for (const auto& [name, table] : tables) {
        ofs << "TABLE " << name << "\n";

        // 儲存欄位資訊，改用 switch 列出所有型態
        for (const auto& col : table.columns) {
            ofs << col.name << " ";
            switch (col.type) {
                case ColumnType::INT: ofs << "INT"; break;
                case ColumnType::STRING: ofs << "STRING"; break;
                case ColumnType::FLOAT: ofs << "FLOAT"; break;
                case ColumnType::DOUBLE: ofs << "DOUBLE"; break;  // ✅ 加這一行
                case ColumnType::BOOL: ofs << "BOOL"; break;
                case ColumnType::DATE: ofs << "DATE"; break;
                default: throw std::runtime_error("Unknown column type");
            }

            ofs << "\n";
        }

        ofs << "DATA\n";

        for (const auto& row : table.rows) {
            for (const auto& val : row.values) {
                if (std::holds_alternative<int>(val)) {
                    ofs << std::get<int>(val);
                } else if (std::holds_alternative<std::string>(val)) {
                    ofs << std::quoted(std::get<std::string>(val));
                } else if (std::holds_alternative<float>(val)) {
                    ofs << std::get<float>(val);
                } else if (std::holds_alternative<double>(val)) {
                    ofs << std::get<double>(val);  // ✅ 補上這一段
                } else if (std::holds_alternative<bool>(val)) {
                    ofs << (std::get<bool>(val) ? "TRUE" : "FALSE");
                } else {
                    throw std::runtime_error("Unknown value type when saving");
                }
                ofs << " ";
            }

            ofs << "\n";
        }

        ofs << "ENDTABLE\n";
    }
}

void Database::load_from_file(const std::string& filename) {
    std::ifstream ifs(filename);
    if (!ifs) throw std::runtime_error("Failed to open file for reading");

    std::string line;
    std::getline(ifs, line);
    if (line != "#INMEMDB_V1") {
        throw std::runtime_error("Invalid file format or missing header");
    }

    while (std::getline(ifs, line)) {
        if (line.rfind("TABLE ", 0) == 0) {
            std::string table_name = line.substr(6);
            std::vector<Column> columns;

            while (std::getline(ifs, line)) {
                if (line == "DATA") break;
                std::istringstream col_iss(line);
                std::string col_name, col_type_str;
                col_iss >> col_name >> col_type_str;

                ColumnType type;
                if (col_type_str == "INT") type = ColumnType::INT;
                else if (col_type_str == "STRING") type = ColumnType::STRING;
                else if (col_type_str == "DOUBLE") type = ColumnType::DOUBLE;
                else if (col_type_str == "FLOAT") type = ColumnType::FLOAT;
                else if (col_type_str == "BOOL") type = ColumnType::BOOL;
                else if (col_type_str == "DATE") type = ColumnType::DATE;
                else throw std::runtime_error("Unknown column type in file: " + col_type_str);

                columns.emplace_back(col_name, type);
            }

            create_table(table_name, columns);
            Table& table = get_table(table_name);

            while (std::getline(ifs, line)) {
                if (line == "ENDTABLE") break;

                std::istringstream row_iss(line);
                std::unordered_map<std::string, Value> row_data;

                for (const auto& col : columns) {
                    switch (col.type) {
                        case ColumnType::INT: {
                            int val;
                            if (!(row_iss >> val)) {
                                throw std::runtime_error("Invalid INT value for column " + col.name);
                            }
                            row_data[col.name] = val;
                            break;
                        }
                        case ColumnType::STRING: {
                            std::string val;
                            if (!(row_iss >> std::quoted(val))) {
                                throw std::runtime_error("Invalid STRING value for column " + col.name);
                            }
                            row_data[col.name] = val;
                            break;
                        }
                        case ColumnType::DOUBLE: {
                            double val;
                            if (!(row_iss >> val)) {
                                throw std::runtime_error("Invalid DOUBLE value for column " + col.name);
                            }
                            row_data[col.name] = val;
                            break;
                        }

                        case ColumnType::FLOAT: {
                            float val;
                            if (!(row_iss >> val)) {
                                throw std::runtime_error("Invalid FLOAT value for column " + col.name);
                            }
                            row_data[col.name] = val;
                            break;
                        }
                        case ColumnType::BOOL: {
                            std::string val;
                            if (!(row_iss >> val)) {
                                throw std::runtime_error("Invalid BOOL value for column " + col.name);
                            }
                            if (val == "TRUE" || val == "true")
                                row_data[col.name] = true;
                            else if (val == "FALSE" || val == "false")
                                row_data[col.name] = false;
                            else
                                throw std::runtime_error("Invalid BOOL literal: " + val);
                            break;
                        }
                        case ColumnType::DATE: {
                            std::string val;
                            if (!(row_iss >> std::quoted(val))) {
                                throw std::runtime_error("Invalid DATE value for column " + col.name);
                            }
                            row_data[col.name] = val;  // 用 string 表示日期
                            break;
                        }
                        default:
                            throw std::runtime_error("Unknown column type during loading");
                    }
                }

                table.insert(row_data);
            }
        }
    }
}



} // namespace db
