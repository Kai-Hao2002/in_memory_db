// include/db/Table.hpp
#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include "Column.hpp"
#include "Row.hpp"
#include "statement/Condition.hpp"
#include <memory> 

namespace db {
class Table {
public:
    Table(const std::string& name, const std::vector<Column>& columns);
    inline bool value_type_matches_column(const Value& val, ColumnType type) {
        switch (type) {
            case ColumnType::INT:
                return std::holds_alternative<int>(val);
            case ColumnType::STRING:
                return std::holds_alternative<std::string>(val);
            case ColumnType::FLOAT:
                return std::holds_alternative<float>(val) || std::holds_alternative<double>(val);
            case ColumnType::DOUBLE:
                return std::holds_alternative<double>(val) || std::holds_alternative<float>(val);
            case ColumnType::BOOL:
                return std::holds_alternative<bool>(val);
            case ColumnType::DATE:
                return std::holds_alternative<std::string>(val);
            default:
                return false;
        }
    }


    void insert(const std::unordered_map<std::string, Value>& data);
    void delete_all();
    void delete_where(std::shared_ptr<statement::Condition> where_condition);
    void update_where(std::shared_ptr<statement::Condition> where_condition,
                  const std::string& set_column, const Value& set_value);
    std::vector<Row> select(const std::vector<std::string>& cols,
                        std::shared_ptr<statement::Condition> where,
                        bool select_all = false) const;  
    // 取得所有 Row 的參考 (非 const)
    std::vector<Row>& get_rows();

    // 取得所有 Row 的 const 版本
    const std::vector<Row>& get_rows() const;

    std::vector<std::string> get_column_names() const {
        std::vector<std::string> col_names;
        for (const auto& col : columns) {
            col_names.push_back(col.name);
        }
        return col_names;
    }

    std::string name;
    std::vector<Column> columns;
    std::vector<Row> rows;
};
}