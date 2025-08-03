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
      
// 輔助函式：根據欄位名稱找到索引
int find_column_index(const std::vector<Column>& columns, const std::string& column_name) {
    for (size_t i = 0; i < columns.size(); ++i) {
        if (columns[i].name == column_name) {
            return static_cast<int>(i);
        }
    }
    throw std::runtime_error("Column not found: " + column_name);
}

// 輔助函式：比較 lhs 和 rhs 是否符合 op 條件
bool compare_values(const Value& lhs, const CompareOp op, const Value& rhs) {
    if (lhs.index() != rhs.index()) {
        throw std::runtime_error("Type mismatch in comparison");
    }

    return std::visit([&](const auto& lval) -> bool {
        const auto& rval = std::get<std::decay_t<decltype(lval)>>(rhs);
        switch (op) {
            case CompareOp::EQ:  return lval == rval;
            case CompareOp::NEQ: return lval != rval;
            case CompareOp::LT:  return lval <  rval;
            case CompareOp::LTE: return lval <= rval;
            case CompareOp::GT:  return lval >  rval;
            case CompareOp::GTE: return lval >= rval;
        }
        return false; // Should never reach here
    }, lhs);
}

// CompareCondition 的 evaluate 實作
bool CompareCondition::evaluate(const Row& row,
                                const std::vector<Column>& columns) const {
    // —— 优化列索引查找 —— 
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

    // —— 类型一致性检查 —— 
    if (cell.index() != value.index())
        throw db::DBException("Type mismatch in comparison for column: " + column);

    // —— 用 std::visit 处理所有类型 & 运算符 —— 
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


// LogicalCondition 的 evaluate 實作
bool LogicalCondition::evaluate(const Row& row, const std::vector<Column>& columns) const {
    bool left_result = left->evaluate(row, columns);
    bool right_result = right->evaluate(row, columns);
    switch (op) {
        case LogicalOp::AND: return left_result && right_result;
        case LogicalOp::OR:  return left_result || right_result;
    }
    return false; // 不可能到這邊
}

// ParenCondition 的 evaluate 實作
bool ParenCondition::evaluate(const Row& row, const std::vector<Column>& columns) const {
    return inner->evaluate(row, columns);
}

} // namespace statement
