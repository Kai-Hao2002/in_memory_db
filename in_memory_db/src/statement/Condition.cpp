//src/statement/Condition.cpp
#include "statement/Condition.hpp"
#include <stdexcept>
#include "exception/DBException.hpp"
#include "util/StringUtil.hpp"  
namespace statement {

using db::Column;
using db::Row;
using db::Value;

CompareCondition::CompareCondition(std::string column,
                                   CompareOp op,
                                   db::Value value)
    : column(db::to_lower(std::move(column))),
      op(op),
      value(std::move(value)) {}
      
// Helper function: find the index based on the field name
int find_column_index(const std::vector<Column>& columns, const std::string& column_name) {
    for (size_t i = 0; i < columns.size(); ++i) {
        if (columns[i].name == column_name) {
            return static_cast<int>(i);
        }
    }
    throw std::runtime_error("Column not found: " + column_name);
}

// Helper function: compare lhs and rhs to see if they match op
bool compare_values(const Value& lhs, CompareOp op, const Value& rhs) {
    return std::visit([&](auto&& lval) -> bool {
        return std::visit([&](auto&& rval) -> bool {
            using L = std::decay_t<decltype(lval)>;
            using R = std::decay_t<decltype(rval)>;

            if constexpr (std::is_arithmetic_v<L> && std::is_arithmetic_v<R>) {
                double lv = static_cast<double>(lval);
                double rv = static_cast<double>(rval);
                switch (op) {
                    case CompareOp::EQ:  return lv == rv;
                    case CompareOp::NEQ: return lv != rv;
                    case CompareOp::LT:  return lv <  rv;
                    case CompareOp::LTE: return lv <= rv;
                    case CompareOp::GT:  return lv >  rv;
                    case CompareOp::GTE: return lv >= rv;
                }
            }

            if constexpr (std::is_same_v<L, std::string> && std::is_same_v<R, std::string>) {
                switch (op) {
                    case CompareOp::EQ:  return lval == rval;
                    case CompareOp::NEQ: return lval != rval;
                    case CompareOp::LT:  return lval <  rval;
                    case CompareOp::LTE: return lval <= rval;
                    case CompareOp::GT:  return lval >  rval;
                    case CompareOp::GTE: return lval >= rval;
                }
            }

            throw db::DBException("Unsupported comparison between types");
        }, rhs);
    }, lhs);
}


// CompareCondition evaluate implementation
bool CompareCondition::evaluate(const Row& row,
                                const std::vector<Column>& columns) const {
    //——Optimize column index lookup——
    int idx = -1;
    for (int i = 0; i < (int)columns.size(); ++i) {
        if (columns[i].name == column) {
            idx = i;
            break;
        }
    }
    if (idx < 0)
        throw db::DBException("Unknown column in condition: " + column);

    const Value& cell = row.values.at(idx);

    //——Type consistency check——
    if (cell.index() != value.index())
        throw db::DBException("Type mismatch in comparison for column: " + column);

    // -- handle all types with std::visit & operator --
    bool result = false;
    std::visit([&](auto const& lval) {
        using T = std::decay_t<decltype(lval)>;
        const T& rval = std::get<T>(value);
        switch (op) {
            case CompareOp::EQ:  result = (lval == rval); break;
            case CompareOp::NEQ: result = (lval != rval); break;
            case CompareOp::LT:  result = (lval <  rval); break;
            case CompareOp::LTE: result = (lval <= rval); break;
            case CompareOp::GT:  result = (lval >  rval); break;
            case CompareOp::GTE: result = (lval >= rval); break;
        }
    }, cell);

    return result;
}


// LogicalCondition's evaluate implementation
bool LogicalCondition::evaluate(const Row& row, const std::vector<Column>& columns) const {
    bool left_result = left->evaluate(row, columns);
    bool right_result = right->evaluate(row, columns);
    switch (op) {
        case LogicalOp::AND: return left_result && right_result;
        case LogicalOp::OR:  return left_result || right_result;
    }
    return false; 
}

//ParenCondition's evaluate implementation
bool ParenCondition::evaluate(const Row& row, const std::vector<Column>& columns) const {
    return inner->evaluate(row, columns);
}

} // namespace statement
