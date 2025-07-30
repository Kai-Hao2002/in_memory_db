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

private:
    std::string table_name_;
    std::vector<db::Column> columns_;
};

}  // namespace statement
