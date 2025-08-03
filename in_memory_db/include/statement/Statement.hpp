// include/statement/Statement.hpp
#pragma once
#include <memory>
#include <vector>
#include <string>
#include "util/Value.hpp"  


namespace db {
class Database;
}

namespace statement {

class Statement {
public:
    virtual ~Statement() = default;
    virtual void execute(db::Database& db) = 0;

    // 回傳語句名稱，例如 "CREATE TABLE"
    virtual std::string name() const = 0;

    // 是否有結果資料 (通常只有 SELECT 有)
    virtual bool has_results() const { return false; }

    // SELECT 專用，取得欄位名稱
    virtual std::vector<std::string> get_columns() const { return {}; }

    // SELECT 專用，取得結果資料
    virtual const std::vector<std::vector<db::Value>>& get_results() const {
        static std::vector<std::vector<db::Value>> empty;
        return empty;
    }
};

using StatementPtr = std::unique_ptr<Statement>;

}  // namespace statement
