// include/parser/Tokenizer.hpp
#pragma once
#include <string>

enum class TokenType {
  Identifier,
  Number,
  StringLiteral,
  Symbol,
  EndOfFile
};

struct Token {
  TokenType type;
  std::string text;
};

class Tokenizer {
public:
  explicit Tokenizer(const std::string& input);

  Token next_token();
  Token peek_token() const;

private:
  void skip_whitespace();
  bool match_keyword(const std::string& word);

  Token consume_token();

  std::string input_;
  size_t pos_ = 0;
};
