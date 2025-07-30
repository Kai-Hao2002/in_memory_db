// include/statement/Select.hpp
#pragma once

#include "statement/Statement.hpp"
#include "db/Database.hpp"
#include "db/Row.hpp"
#include <string>
#include <vector>
#include <optional>

namespace statement {

class Select : public Statement {
public:
    Select(const std::string& table_name,
           const std::vector<std::string>& columns,
           const std::optional<std::pair<std::string, db::Value>>& where = std::nullopt);

    void execute(db::Database& db) override;

private:
    std::string table_name_;
    std::vector<std::string> columns_;
    std::optional<std::pair<std::string, db::Value>> where_;
};

}  // namespace statement
