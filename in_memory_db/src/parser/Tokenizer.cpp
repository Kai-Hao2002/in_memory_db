// src/parser/Tokenizer.cpp
#include "parser/Tokenizer.hpp"
#include <cctype>
#include <stdexcept>
#include <algorithm>

Tokenizer::Tokenizer(const std::string& input) : input_(input), pos_(0) {}

void Tokenizer::skip_whitespace() {
  while (pos_ < input_.size() && std::isspace(input_[pos_])) {
    ++pos_;
  }
}

bool Tokenizer::match_keyword(const std::string& word) {
  size_t len = word.length();
  if (pos_ + len <= input_.size() &&
      input_.compare(pos_, len, word) == 0) {
    if (pos_ + len == input_.size() || !std::isalnum(input_[pos_ + len])) {
      pos_ += len;
      return true;
    }
  }
  return false;
}

Token Tokenizer::next_token() {
  skip_whitespace();

  if (pos_ >= input_.size()) {
    return {TokenType::EndOfFile, ""};
  }

  char c = input_[pos_];

  // ⭐ 雙字元運算子先判斷（像 !=、<=、>=）
  if (pos_ + 1 < input_.size()) {
    std::string two_chars = input_.substr(pos_, 2);
    if (two_chars == "!=" || two_chars == "<=" || two_chars == ">=") {
      pos_ += 2;
      return {TokenType::Symbol, two_chars};
    }
  }

  // ⭐ 單字符符號
  if (c == ',' || c == '(' || c == ')' || c == '=' || c == '*' || c == '<' || c == '>' || c == '.' || c == ';') {
      ++pos_;
      return {TokenType::Symbol, std::string(1, c)};
  }




  // 字串字面量，支援單引號與雙引號
  if (c == '\'' || c == '"') {
      char quote = c;
      ++pos_;
      std::string str;
      while (pos_ < input_.size() && input_[pos_] != quote) {
          str += input_[pos_++];
      }
      if (pos_ == input_.size()) {
          throw std::runtime_error("Unterminated string literal");
      }
      ++pos_; // 跳過結尾引號
      return {TokenType::StringLiteral, str};
  }


  // 數值字面量（整數 or 浮點數）
  if (std::isdigit(c)) {
      std::string num;
      bool has_dot = false;
      while (pos_ < input_.size() && (std::isdigit(input_[pos_]) || input_[pos_] == '.')) {
          if (input_[pos_] == '.') {
              if (has_dot) break;  // 第二個點就停止，避免錯誤格式
              has_dot = true;
          }
          num += input_[pos_++];
      }
      return {TokenType::Number, num};  // Parser 根據是否有 . 再做 float/double/int 判斷
  }


  // 識別字(關鍵字也是用這個標示)
  if (std::isalpha(c) || c == '_') {
    std::string id;
    while (pos_ < input_.size() &&
          (std::isalnum(input_[pos_]) || input_[pos_] == '_')) {
      id += std::toupper(input_[pos_++]);  // 轉大寫方便後續判斷關鍵字
    }

    if (id == "TRUE" || id == "FALSE") {
      return {TokenType::BooleanLiteral, id};
    }

    return {TokenType::Identifier, id};
  }


  throw std::runtime_error(std::string("Unknown character: ") + c);
}

Token Tokenizer::peek_token() const {
  auto* self = const_cast<Tokenizer*>(this);
  size_t saved_pos = self->pos_;
  Token t = self->next_token();
  self->pos_ = saved_pos;
  return t;
}
