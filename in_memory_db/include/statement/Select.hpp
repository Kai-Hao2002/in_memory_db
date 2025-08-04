// include/statement/Select.hpp
#pragma once

#include "statement/Statement.hpp"
#include "db/Database.hpp"
#include "util/Value.hpp"
#include "statement/Condition.hpp"
#include <string>
#include <vector>
#include <memory>

namespace statement {

class Select : public Statement {
public:
    Select(const std::string& table_name,
           const std::vector<std::string>& columns,
           std::shared_ptr<Condition> where = nullptr,
           bool select_all = false);

    void execute(db::Database& db) override;

    std::string name() const override { return "SELECT"; }

    bool has_results() const override { return true; }

    // Return the parsed column names instead of the original input columns_
    std::vector<std::string> get_columns() const override { return output_column_names_; }

    const std::vector<std::vector<db::Value>>& get_results() const override { return results_; }

    const std::string& table_name() const { return table_name_; }
    const std::vector<std::string>& columns() const { return columns_; }
    std::shared_ptr<Condition> where() const { return where_; }
    bool is_select_all() const { return select_all_; }
    const std::vector<std::string>& get_output_columns() const { return output_column_names_; }

private:
    std::string table_name_;
    std::vector<std::string> columns_; // Original input (maybe "*")
    std::shared_ptr<Condition> where_;
    bool select_all_ = false;

    std::vector<std::vector<db::Value>> results_;             //Data column
    std::vector<std::string> output_column_names_;            // The parsed column name (the column name to be displayed)
};


}  // namespace statement
