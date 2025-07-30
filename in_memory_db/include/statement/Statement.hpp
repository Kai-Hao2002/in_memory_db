// include/statement/Statement.hpp
#pragma once
#include <memory>

namespace db {
class Database;
}

namespace statement {

class Statement {
public:
    virtual ~Statement() = default;

    virtual void execute(db::Database& db) = 0;
};

using StatementPtr = std::unique_ptr<Statement>;

}  // namespace statement
