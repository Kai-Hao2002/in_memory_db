// include/db/Table.hpp
#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>

#include "Column.hpp"
#include "Row.hpp"

namespace db {
class Table {
public:
    Table(const std::string& name, const std::vector<Column>& columns);

    void insert(const std::unordered_map<std::string, Value>& data);
    void delete_all();
    void delete_where(const std::string& column, const Value& value);
    void update_where(const std::string& where_column, const Value& where_value,
                      const std::string& set_column, const Value& set_value);
    std::vector<Row> select(const std::vector<std::string>& col_names,
                            const std::optional<std::pair<std::string, Value>>& where = std::nullopt) const;
    
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