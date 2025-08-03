// include/statement/CreateTable.hpp
#pragma once

#include "statement/Statement.hpp"
#include "db/Column.hpp"
#include <string>
#include <vector>

namespace statement {

class CreateTableStatement : public Statement {
public:
    CreateTableStatement(std::string table_name, std::vector<db::Column> columns);

    void execute(db::Database& db) override;
    const std::string& table_name() const { return table_name_; }
    const std::vector<db::Column>& columns() const { return columns_; }

    std::string name() const override { return "CREATE TABLE"; }

private:
    std::string table_name_;
    std::vector<db::Column> columns_;
};

}  // namespace statement
