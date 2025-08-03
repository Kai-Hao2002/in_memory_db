// include/statement/Update.hpp
#pragma once

#include "statement/Statement.hpp"
#include "statement/Condition.hpp"
#include "db/Database.hpp"
#include "db/Row.hpp"
#include <string>
#include <memory>

namespace statement {

class Update : public Statement {
public:
    // 使用 Condition 來支援複雜條件
    Update(const std::string& table_name,
           const std::string& set_column,
           const db::Value& set_value,
           std::shared_ptr<Condition> where_condition = nullptr);

    void execute(db::Database& db) override;

    const std::string& table_name() const { return table_name_; }
    const std::string& set_column() const { return set_column_; }
    const db::Value& set_value() const { return set_value_; }
    const std::shared_ptr<Condition>& where_condition() const { return where_condition_; }
    
    std::string name() const override { return "UPDATE"; }
    
private:
    std::string table_name_;
    std::string set_column_;
    db::Value set_value_;
    std::shared_ptr<Condition> where_condition_;
};

}  // namespace statement
