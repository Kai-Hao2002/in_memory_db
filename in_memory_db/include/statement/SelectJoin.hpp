// include/statement/SelectJoin.hpp
#pragma once
#include "statement/Statement.hpp"
#include "db/Database.hpp"
#include "statement/Condition.hpp"
#include <string>
#include <vector>
#include <memory>

namespace statement {

class SelectJoin : public Statement {
public:
// SelectJoin.hpp
    SelectJoin(const std::string& left_table,
            const std::string& right_table,
            const std::vector<std::string>& select_columns,
            const std::string& left_join_column,
            const std::string& right_join_column,
            std::shared_ptr<Condition> where = nullptr,
            bool select_all = false);

    

    void execute(db::Database& db) override;

    std::string name() const override { return "SELECT_JOIN"; }

    bool has_results() const override { return true; }

    std::vector<std::string> get_columns() const override { return output_column_names_; }

    const std::vector<std::vector<db::Value>>& get_results() const override { return results_; }

    const std::vector<std::string>& get_output_column_names() const { return output_column_names_; }
    
private:
    std::string left_table_;
    std::string right_table_;
    std::vector<std::string> select_columns_;
    std::string left_join_column_;   // Without table name, such as "colA"
    std::string right_join_column_;  // Without table name, such as "colB"
    std::shared_ptr<Condition> where_;

    std::vector<std::vector<db::Value>> results_;
    std::vector<std::string> output_column_names_;
    bool select_all_ = false;
};

}  // namespace statement
