// src/parser/Parser.cpp
#include <string> 
#include <vector>
#include <memory>
#include <optional>
#include <stdexcept> 
#include <algorithm> 

#include "db/Column.hpp"
#include "statement/CreateTable.hpp"
#include "statement/Insert.hpp"
#include "statement/Select.hpp"
#include "statement/Update.hpp"
#include "statement/Delete.hpp"
#include "statement/SelectJoin.hpp"
#include "parser/Parser.hpp"
#include "util/StringUtil.hpp"



Parser::Parser(const std::string& sql) : tokenizer_(sql) {}



statement::StatementPtr Parser::parse() { 
  Token token = peek_token(); 
  if (token.type == TokenType::Identifier) { 
    std::string kw = token.text; 
    std::transform(kw.begin(), kw.end(), kw.begin(), ::toupper); 
    statement::StatementPtr stmt;

    if (kw == "CREATE") stmt = parse_create_table(); 
    else if (kw == "INSERT") stmt = parse_insert();
    else if (kw == "SELECT") stmt = parse_select();
    else if (kw == "UPDATE") stmt = parse_update();
    else if (kw == "DELETE") stmt = parse_delete();
    else
      throw std::runtime_error("Unsupported or invalid SQL command.");

    // 嘗試忽略分號
    Token next = peek_token();
    if (next.type == TokenType::Symbol && next.text == ";") {
      consume_token(); // consume ';' but ignore
    }

    return stmt;
  } 
  throw std::runtime_error("Unsupported or invalid SQL command."); 
}


// 解析 create_table table_name (col1, col2, ...) VALUES (val1, val2, ...)
statement::StatementPtr Parser::parse_create_table() {
  // consume CREATE
  expect_token(TokenType::Identifier, "CREATE");

  // consume TABLE
  expect_token(TokenType::Identifier, "TABLE");

  // table name
  Token table_name_token = consume_token();
  if (table_name_token.type != TokenType::Identifier) {
    throw std::runtime_error("Expected table name after CREATE TABLE");
  }
  std::string table_name = table_name_token.text;

  // consume '('
  expect_token(TokenType::Symbol, "(");

  std::vector<db::Column> columns;

  while (true) {
    // parse column name
    Token col_name_token = consume_token();
    if (col_name_token.type != TokenType::Identifier) {
      throw std::runtime_error("Expected column name");
    }
    std::string col_name = col_name_token.text;

    // parse column type
    Token col_type_token = consume_token();
    if (col_type_token.type != TokenType::Identifier) {
      throw std::runtime_error("Expected column type");
    }
    

    db::ColumnType type;
    std::string col_type = db::to_lower(col_type_token.text);

    if (col_type == "int") type = db::ColumnType::INT;
    else if (col_type == "string") type = db::ColumnType::STRING;
    else if (col_type == "float") type = db::ColumnType::FLOAT;
    else if (col_type == "double") type = db::ColumnType::DOUBLE;
    else if (col_type == "bool") type = db::ColumnType::BOOL;
    else throw std::runtime_error("Unsupported column type: " + col_type);


    columns.emplace_back(col_name, type);

    Token next = peek_token();
    if (next.type == TokenType::Symbol && next.text == ",") {
      consume_token(); // consume ','
      continue;
    } else if (next.type == TokenType::Symbol && next.text == ")") {
      consume_token(); // consume ')'
      break;
    } else {
      throw std::runtime_error("Expected ',' or ')' in column definition");
    }
  }

  return std::make_unique<statement::CreateTableStatement>(table_name, columns);
}

// 解析 INSER INTO table_name (col1, col2, ...) VALUES (val1, val2, ...)
statement::StatementPtr Parser::parse_insert() {
  expect_token(TokenType::Identifier, "INSERT");
  expect_token(TokenType::Identifier, "INTO");

  Token table_token = consume_token();
  if (table_token.type != TokenType::Identifier) {
    throw std::runtime_error("Expected table name after INSERT INTO");
  }
  std::string table_name = table_token.text;

  expect_token(TokenType::Symbol, "(");

  std::vector<std::string> columns;
  while (true) {
    Token col_token = consume_token();
    if (col_token.type != TokenType::Identifier) {
      throw std::runtime_error("Expected column name in INSERT");
    }
    columns.push_back(col_token.text);

    Token next = peek_token();
    if (next.type == TokenType::Symbol && next.text == ",") {
      consume_token();
    } else if (next.type == TokenType::Symbol && next.text == ")") {
      consume_token();
      break;
    } else {
      throw std::runtime_error("Expected ',' or ')' in column list");
    }
  }

  expect_token(TokenType::Identifier, "VALUES");
  expect_token(TokenType::Symbol, "(");

  std::vector<db::Value> values;
  while (true) {
    Token val_token = consume_token();
    std::cout << "val_token.type=" << static_cast<int>(val_token.type)
              << " val_token.text='" << val_token.text << "'" << std::endl;

    if (val_token.type == TokenType::Number) {
        if (val_token.text.find('.') != std::string::npos) {
            values.emplace_back(std::stod(val_token.text));
        } else {
            values.emplace_back(std::stoi(val_token.text));
        }
    } else if (val_token.type == TokenType::StringLiteral) {
        values.emplace_back(val_token.text);
    } else if (val_token.type == TokenType::Identifier) {
        std::string val_lower = db::to_lower(val_token.text);
        if (val_lower == "true") {
            values.emplace_back(true);
        } else if (val_lower == "false") {
            values.emplace_back(false);
        } else {
            throw std::runtime_error("Unexpected identifier as value: " + val_token.text);
        }
    } else if (val_token.type == TokenType::BooleanLiteral) {
        std::string val_lower = db::to_lower(val_token.text);
        if (val_lower == "true") {
            values.emplace_back(true);
        } else if (val_lower == "false") {
            values.emplace_back(false);
        } else {
            throw std::runtime_error("Unexpected boolean literal: " + val_token.text);
        }
    } else {
        throw std::runtime_error("Expected value in VALUES");
    }

    Token next = peek_token();
    if (next.type == TokenType::Symbol && next.text == ",") {
      consume_token();
    } else if (next.type == TokenType::Symbol && next.text == ")") {
      consume_token();
      break;
    } else {
      throw std::runtime_error("Expected ',' or ')' in value list");
    }
  }

  std::unordered_map<std::string, db::Value> row;
  for (size_t i = 0; i < columns.size(); ++i) {
      row[db::to_lower(columns[i])] = std::move(values[i]);
  }
  std::vector<std::unordered_map<std::string, db::Value>> rows = { row };

  return std::make_unique<statement::Insert>(table_name, rows);

}

// 解析 Select 
statement::StatementPtr Parser::parse_select() {
  expect_token(TokenType::Identifier, "SELECT");

  // 解析欄位列表
  std::vector<std::string> columns;
  bool select_all = false;
  Token token = peek_token();

  if (token.type == TokenType::Symbol && token.text == "*") {
      consume_token();
      columns.push_back("*");
      select_all = true;
  } else {
      while (true) {
          Token col_token = consume_token();
          if (col_token.type != TokenType::Identifier) {
              throw std::runtime_error("Expected column name in SELECT");
          }
          std::string col_name = col_token.text;

          // 檢查是否有 table.column 格式
          if (peek_token().type == TokenType::Symbol && peek_token().text == ".") {
              consume_token(); // consume '.'
              Token next_col_token = consume_token();
              if (next_col_token.type != TokenType::Identifier) {
                  throw std::runtime_error("Expected column name after '.' in SELECT");
              }
              col_name += "." + next_col_token.text;
          }

          columns.push_back(col_name);

          Token next = peek_token();
          if (next.type == TokenType::Symbol && next.text == ",") {
              consume_token();
          } else {
              break;
          }
      }
  }


  // 讀取 FROM
  expect_token(TokenType::Identifier, "FROM");

  Token table_token = consume_token();
  if (table_token.type != TokenType::Identifier) {
    throw std::runtime_error("Expected table name after FROM");
  }
  std::string table_name = table_token.text;

  Token next = peek_token();

  // INNER JOIN 解析
  if (next.type == TokenType::Identifier && 
      (next.text == "INNER" || next.text == "inner")) {

    consume_token(); // consume INNER

    expect_token(TokenType::Identifier, "JOIN");

    // 讀取第二個表名
    Token table2_token = consume_token();
    if (table2_token.type != TokenType::Identifier) {
      throw std::runtime_error("Expected table name after INNER JOIN");
    }
    std::string table2_name = table2_token.text;

    expect_token(TokenType::Identifier, "ON");

    // 解析 JOIN 條件，格式：table1.col1 = table2.col2
    // 左邊欄位，允許 table.column 格式
    Token left_table_token = consume_token();
    if (left_table_token.type != TokenType::Identifier) {
      throw std::runtime_error("Expected table name or column name in JOIN condition");
    }
    std::string left_col = left_table_token.text;
    if (peek_token().type == TokenType::Symbol && peek_token().text == ".") {
      consume_token(); // consume '.'
      Token col_name_token = consume_token();
      if (col_name_token.type != TokenType::Identifier) {
        throw std::runtime_error("Expected column name after '.' in JOIN condition");
      }
      left_col += "." + col_name_token.text;
    }

    expect_token(TokenType::Symbol, "=");

    // 右邊欄位，允許 table.column 格式
    Token right_table_token = consume_token();
    if (right_table_token.type != TokenType::Identifier) {
      throw std::runtime_error("Expected table name or column name in JOIN condition");
    }
    std::string right_col = right_table_token.text;
    if (peek_token().type == TokenType::Symbol && peek_token().text == ".") {
      consume_token(); // consume '.'
      Token col_name_token = consume_token();
      if (col_name_token.type != TokenType::Identifier) {
        throw std::runtime_error("Expected column name after '.' in JOIN condition");
      }
      right_col += "." + col_name_token.text;
    }

    // 讀取可選的 WHERE 條件
    std::shared_ptr<statement::Condition> where_condition = nullptr;
    Token after_join = peek_token();
    if (after_join.type == TokenType::Identifier && 
        (after_join.text == "WHERE" || after_join.text == "where")) {
      consume_token();
      where_condition = parse_condition();
    }

    // 回傳 SelectJoin 物件
    return std::make_unique<statement::SelectJoin>(
      table_name, table2_name, columns, left_col, right_col, where_condition, select_all);
  }

  // 沒有 JOIN，解析可選的 WHERE
  std::shared_ptr<statement::Condition> where_condition = nullptr;
  if (next.type == TokenType::Identifier && 
      (next.text == "WHERE" || next.text == "where")) {
    consume_token();
    where_condition = parse_condition();
  }

  return std::make_unique<statement::Select>(table_name, columns, where_condition, select_all);
}



// 解析 UPDATE table_name SET col = val WHERE col = val
statement::StatementPtr Parser::parse_update() {
  expect_token(TokenType::Identifier, "UPDATE");

  Token table_token = consume_token();
  if (table_token.type != TokenType::Identifier)
    throw std::runtime_error("Expected table name after UPDATE");
  std::string table_name = table_token.text;

  expect_token(TokenType::Identifier, "SET");

  Token set_col = consume_token();
  if (set_col.type != TokenType::Identifier)
    throw std::runtime_error("Expected column name after SET");

  expect_token(TokenType::Symbol, "=");

  Token set_val_token = consume_token();
  db::Value set_val;
  if (set_val_token.type == TokenType::Number) {
    set_val = std::stoi(set_val_token.text);
  } else if (set_val_token.type == TokenType::StringLiteral) {
    set_val = set_val_token.text;
  } else {
    throw std::runtime_error("Expected value after '=' in SET");
  }

  std::shared_ptr<statement::Condition> where_condition = nullptr;
  Token next = peek_token();
  if (next.type == TokenType::Identifier && (next.text == "WHERE" || next.text == "where")) {
    consume_token();  // consume WHERE
    where_condition = parse_condition();
  }

  return std::make_unique<statement::Update>(table_name, set_col.text, set_val, where_condition);
}

// 解析 DELETE FROM table_name WHERE col = val
statement::StatementPtr Parser::parse_delete() {
  expect_token(TokenType::Identifier, "DELETE");
  expect_token(TokenType::Identifier, "FROM");

  Token table_token = consume_token();
  if (table_token.type != TokenType::Identifier)
    throw std::runtime_error("Expected table name after DELETE FROM");
  std::string table_name = table_token.text;

  std::shared_ptr<statement::Condition> where_condition = nullptr;
  Token next = peek_token();
  if (next.type == TokenType::Identifier && (next.text == "WHERE" || next.text == "where")) {
    consume_token();  // consume WHERE
    where_condition = parse_condition();
  }

  return std::make_unique<statement::Delete>(table_name, where_condition);
}

void Parser::expect_token(TokenType type, const std::string& text) {
  Token token = consume_token();
  if (token.type != type || (!text.empty() && token.text != text)) {
    throw std::runtime_error("Unexpected token: " + token.text);
  }
}

Token Parser::consume_token() {
  return tokenizer_.next_token();
}

Token Parser::peek_token() {
  return tokenizer_.peek_token();
}

// parse OR 項目： left OR right OR ...
std::shared_ptr<statement::Condition> Parser::parse_logical_or() {
  auto left = parse_logical_and();
  while (true) {
    Token token = peek_token();
    if (token.type == TokenType::Identifier &&
        (token.text == "OR" || token.text == "or")) {
      consume_token(); // consume OR
      auto right = parse_logical_and();
      left = std::make_shared<statement::LogicalCondition>(
          statement::LogicalOp::OR, left, right);
    } else {
      break;
    }
  }
  return left;
}

// parse AND 項目： left AND right AND ...
std::shared_ptr<statement::Condition> Parser::parse_logical_and() {
  auto left = parse_primary();
  while (true) {
    Token token = peek_token();
    if (token.type == TokenType::Identifier &&
        (token.text == "AND" || token.text == "and")) {
      consume_token(); // consume AND
      auto right = parse_primary();
      left = std::make_shared<statement::LogicalCondition>(
          statement::LogicalOp::AND, left, right);
    } else {
      break;
    }
  }
  return left;
}

// parse_primary: 括號或比較條件
std::shared_ptr<statement::Condition> Parser::parse_primary() {
  Token token = peek_token();

  if (token.type == TokenType::Symbol && token.text == "(") {
    consume_token();  // consume '('
    auto cond = parse_condition();
    expect_token(TokenType::Symbol, ")");
    return std::make_shared<statement::ParenCondition>(cond);
  } else {
    return parse_comparison();
  }
}

// parse_comparison: 比較條件 col op val
std::shared_ptr<statement::Condition> Parser::parse_comparison() {
  Token col_token = consume_token();
  if (col_token.type != TokenType::Identifier) {
    throw std::runtime_error("Expected column name in condition");
  }
  std::string column = col_token.text;

  auto op = parse_compare_op();

  Token val_token = consume_token();
  db::Value value;
  if (val_token.type == TokenType::Number) {
    value = std::stoi(val_token.text);
  } else if (val_token.type == TokenType::StringLiteral) {
    value = val_token.text;
  } else if (val_token.type == TokenType::BooleanLiteral) {
    // 假設你有 boolean literal 的 token type，並且 Value 能接受 bool
    value = (val_token.text == "TRUE" || val_token.text == "true");
  } else {
    throw std::runtime_error("Expected number, string, or boolean literal but got token type: " + std::to_string(static_cast<int>(val_token.type)));
  }

  return std::make_shared<statement::CompareCondition>(column, op, value);
}


// parse_compare_op: 取得比較運算子
statement::CompareOp Parser::parse_compare_op() {
  Token token = consume_token();
  if (token.type != TokenType::Symbol)
    throw std::runtime_error("Expected comparison operator");

  if (token.text == "=") return statement::CompareOp::EQ;
  else if (token.text == "!=") return statement::CompareOp::NEQ;
  else if (token.text == "<") return statement::CompareOp::LT;
  else if (token.text == "<=") return statement::CompareOp::LTE;
  else if (token.text == ">") return statement::CompareOp::GT;
  else if (token.text == ">=") return statement::CompareOp::GTE;

  throw std::runtime_error("Unknown comparison operator: " + token.text);
}

// parse_logical_op: 取得邏輯運算子 AND / OR
statement::LogicalOp Parser::parse_logical_op() {
  Token token = consume_token();
  if (token.type != TokenType::Identifier)
    throw std::runtime_error("Expected logical operator AND or OR");

  if (token.text == "AND" || token.text == "and") return statement::LogicalOp::AND;
  if (token.text == "OR" || token.text == "or") return statement::LogicalOp::OR;

  throw std::runtime_error("Unknown logical operator: " + token.text);
}

std::shared_ptr<statement::Condition> Parser::parse_condition() {
  return parse_logical_or();
}