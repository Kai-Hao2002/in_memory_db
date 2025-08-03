// include/parser/Parser.hpp
#pragma once
#include "Tokenizer.hpp"
#include "statement/Statement.hpp"
#include "statement/Condition.hpp"
#include <memory>

class Parser {
public:
  explicit Parser(const std::string& sql);
  
  statement::StatementPtr parse();

private:
  Tokenizer tokenizer_;
  
  statement::StatementPtr parse_create_table();
  statement::StatementPtr parse_insert();
  statement::StatementPtr parse_select();
  statement::StatementPtr parse_update();
  statement::StatementPtr parse_delete();

    // ✅ condition parse
  std::shared_ptr<statement::Condition> parse_condition();
  std::shared_ptr<statement::Condition> parse_logical_or();
  std::shared_ptr<statement::Condition> parse_logical_and();
  std::shared_ptr<statement::Condition> parse_comparison();
  std::shared_ptr<statement::Condition> parse_primary();

  // ✅ operator parse
  statement::CompareOp parse_compare_op();
  statement::LogicalOp parse_logical_op();

  void expect_token(TokenType type, const std::string& text = "");
  Token consume_token();
  Token peek_token();
};
