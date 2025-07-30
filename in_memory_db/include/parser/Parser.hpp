// include/parser/Parser.hpp
#pragma once
#include "Tokenizer.hpp"
#include "statement/Statement.hpp"
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

  void expect_token(TokenType type, const std::string& text = "");
  Token consume_token();
  Token peek_token();
};
