// include/statement/Delete.hpp
#pragma once
#include "statement/Statement.hpp"
#include <optional>
#include <string>
#include "db/Row.hpp"     // 取得 db::Value

namespace statement {

class Delete : public Statement {
public:
    Delete(std::string table_name,
           std::optional<std::pair<std::string, db::Value>> where = std::nullopt);

    void execute(db::Database& db) override;

private:
    std::string table_name_;
    std::optional<std::pair<std::string, db::Value>> where_;
};

}  // namespace statement
