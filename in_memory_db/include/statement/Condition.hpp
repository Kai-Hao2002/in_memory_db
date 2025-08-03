//include/statement/Condition.hpp
#pragma once
#include <memory>
#include <string>
#include "db/Row.hpp"
#include "db/Column.hpp"
#include "util/Value.hpp"


namespace statement {

enum class CompareOp { EQ, NEQ, LT, LTE, GT, GTE };
enum class LogicalOp { AND, OR };

struct Condition {
    virtual ~Condition() = default;
    virtual bool evaluate(const db::Row&, const std::vector<db::Column>&) const = 0;
};

struct CompareCondition : public Condition {
    std::string column;
    CompareOp op;
    db::Value value;

    CompareCondition(std::string column, CompareOp op, db::Value value);


    bool evaluate(const db::Row&, const std::vector<db::Column>&) const override;
};

struct LogicalCondition : public Condition {
    LogicalOp op;
    std::shared_ptr<Condition> left;
    std::shared_ptr<Condition> right;

    LogicalCondition(LogicalOp op,
                     std::shared_ptr<Condition> left,
                     std::shared_ptr<Condition> right)
        : op(op), left(std::move(left)), right(std::move(right)) {}

    bool evaluate(const db::Row&, const std::vector<db::Column>&) const override;
};

struct ParenCondition : public Condition {
    std::shared_ptr<Condition> inner;

    explicit ParenCondition(std::shared_ptr<Condition> inner)
        : inner(std::move(inner)) {}

    bool evaluate(const db::Row&, const std::vector<db::Column>&) const override;
};

}  // namespace statement
