// include/statement/Delete.hpp
#pragma once
#include "statement/Statement.hpp"
#include "statement/Condition.hpp"
#include <memory>
#include <string>

namespace statement {

class Delete : public Statement {
public:
    // 將條件改為 shared_ptr<Condition>，nullptr 表示無條件
    Delete(std::string table_name, std::shared_ptr<Condition> where_condition = nullptr);
    void execute(db::Database& db) override;

    const std::string& table_name() const { return table_name_; }
    std::shared_ptr<Condition> where_condition() const { return where_condition_; }    

    std::string name() const override { return "DELETE FROM"; }

private:
    std::string table_name_;
    std::shared_ptr<Condition> where_condition_;
};

}  // namespace statement
