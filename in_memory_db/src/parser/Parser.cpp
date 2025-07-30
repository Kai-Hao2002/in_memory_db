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
#include "parser/Parser.hpp"

Parser::Parser(const std::string& sql) : tokenizer_(sql) {}



statement::StatementPtr Parser::parse() { 
  Token token = peek_token(); 
  if (token.type == TokenType::Identifier) { 
    std::string kw = token.text; 
    std::transform(kw.begin(), kw.end(), kw.begin(), ::toupper); 
    if (kw == "CREATE") return parse_create_table(); 
    else if (kw == "INSERT") return parse_insert();
    else if (kw == "SELECT") return parse_select();
    else if (kw == "UPDATE") return parse_update();
    else if (kw == "DELETE") return parse_delete();
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
    std::string col_type = col_type_token.text;

    db::ColumnType type;
    if (col_type == "INT") type = db::ColumnType::INT;
    else if (col_type == "STRING") type = db::ColumnType::STRING;
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


// 解析 INSERT INTO table_name (col1, col2, ...) VALUES (val1, val2, ...)
statement::StatementPtr Parser::parse_select() {
  expect_token(TokenType::Identifier, "SELECT");

  std::vector<std::string> columns;
  Token token = peek_token();

  if (token.type == TokenType::Symbol && token.text == "*") {
    // 遇到 * 代表選取所有欄位
    consume_token();
    columns.push_back("*");
  } else {
    // 否則讀取欄位名稱列表
    while (true) {
      Token col_token = consume_token();
      if (col_token.type != TokenType::Identifier) {
        throw std::runtime_error("Expected column name in SELECT");
      }
      columns.push_back(col_token.text);

      Token next = peek_token();
      if (next.type == TokenType::Symbol && next.text == ",") {
        consume_token(); // 跳過逗號，繼續讀下一欄
      } else {
        break; // 欄位列表結束
      }
    }
  }

  expect_token(TokenType::Identifier, "FROM");

  Token table_token = consume_token();
  if (table_token.type != TokenType::Identifier) {
    throw std::runtime_error("Expected table name after FROM");
  }
  std::string table_name = table_token.text;

  std::optional<std::pair<std::string, db::Value>> where_clause = std::nullopt;

  Token next = peek_token();
  if (next.type == TokenType::Identifier &&
      (next.text == "WHERE" || next.text == "where")) {
    consume_token();  // consume WHERE

    Token where_col = consume_token();
    if (where_col.type != TokenType::Identifier) {
      throw std::runtime_error("Expected column name after WHERE");
    }

    Token eq = consume_token();
    if (eq.type != TokenType::Symbol || eq.text != "=") {
      throw std::runtime_error("Expected '=' after WHERE column");
    }

    Token val = consume_token();
    db::Value val_obj;
    if (val.type == TokenType::Number) {
      val_obj = std::stoi(val.text);
    } else if (val.type == TokenType::StringLiteral) {
      val_obj = val.text;
    } else {
      throw std::runtime_error("Expected value after '=' in WHERE clause");
    }

    where_clause = std::make_pair(where_col.text, val_obj);
  }

  return std::make_unique<statement::Select>(table_name, columns, where_clause);
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

  expect_token(TokenType::Identifier, "WHERE");

  Token where_col = consume_token();
  if (where_col.type != TokenType::Identifier)
    throw std::runtime_error("Expected column name after WHERE");

  expect_token(TokenType::Symbol, "=");

  Token where_val_token = consume_token();
  db::Value where_val;
  if (where_val_token.type == TokenType::Number) {
    where_val = std::stoi(where_val_token.text);
  } else if (where_val_token.type == TokenType::StringLiteral) {
    where_val = where_val_token.text;
  } else {
    throw std::runtime_error("Expected value after '=' in WHERE");
  }

  return std::make_unique<statement::Update>(
    table_name, set_col.text, set_val, where_col.text, where_val);
}

// 解析 DELETE FROM table_name WHERE col = val
statement::StatementPtr Parser::parse_delete() {
  expect_token(TokenType::Identifier, "DELETE");
  expect_token(TokenType::Identifier, "FROM");

  Token table_token = consume_token();
  if (table_token.type != TokenType::Identifier)
    throw std::runtime_error("Expected table name after DELETE FROM");
  std::string table_name = table_token.text;

  expect_token(TokenType::Identifier, "WHERE");

  Token where_col = consume_token();
  if (where_col.type != TokenType::Identifier)
    throw std::runtime_error("Expected column name after WHERE");

  expect_token(TokenType::Symbol, "=");

  Token where_val_token = consume_token();
  db::Value where_val;
  if (where_val_token.type == TokenType::Number) {
    where_val = std::stoi(where_val_token.text);
  } else if (where_val_token.type == TokenType::StringLiteral) {
    where_val = where_val_token.text;
  } else {
    throw std::runtime_error("Expected value after '=' in WHERE");
  }

  return std::make_unique<statement::Delete>(table_name,
           std::optional<std::pair<std::string, db::Value>>{
               std::make_pair(where_col.text, where_val)});
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