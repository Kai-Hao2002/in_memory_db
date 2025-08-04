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

    // Returns the statement name, such as "CREATE TABLE"
    virtual std::string name() const = 0;

    // Is there any result data (usually only SELECT)
    virtual bool has_results() const { return false; }

    // SELECT only, get the column name
    virtual std::vector<std::string> get_columns() const { return {}; }

    // SELECT only, get result data
    virtual const std::vector<std::vector<db::Value>>& get_results() const {
        static std::vector<std::vector<db::Value>> empty;
        return empty;
    }
};

using StatementPtr = std::unique_ptr<Statement>;

}  // namespace statement
