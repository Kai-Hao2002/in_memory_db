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

    const std::string& table_name() const { return table_name_; }
    const std::vector<std::unordered_map<std::string, db::Value>>& rows() const { return rows_; }

    std::string name() const override { return "INSERT"; }
    
private:
    std::string table_name_;
    std::vector<std::unordered_map<std::string, db::Value>> rows_;
};

}  // namespace statement
