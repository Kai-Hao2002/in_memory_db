// include/statement/Insert.hpp
#pragma once

#include "statement/Statement.hpp"
#include "db/Row.hpp"
#include <string>
#include <vector>
#include <unordered_map>

namespace statement {

class Insert : public Statement {
public:
    Insert(std::string table_name, std::vector<std::unordered_map<std::string, db::Value>> rows);

    void execute(db::Database& db) override;

private:
    std::string table_name_;
    std::vector<std::unordered_map<std::string, db::Value>> rows_;
};

}  // namespace statement
