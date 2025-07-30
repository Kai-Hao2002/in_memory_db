// include/statement/Update.hpp
#pragma once

#include "statement/Statement.hpp"
#include "db/Database.hpp"
#include "db/Row.hpp"
#include <string>

namespace statement {

class Update : public Statement {
public:
    Update(const std::string& table_name,
           const std::string& set_column,
           const db::Value& set_value,
           const std::string& where_column,
           const db::Value& where_value);

    void execute(db::Database& db) override;

private:
    std::string table_name_;
    std::string set_column_;
    db::Value set_value_;
    std::string where_column_;
    db::Value where_value_;
};

}  // namespace statement
