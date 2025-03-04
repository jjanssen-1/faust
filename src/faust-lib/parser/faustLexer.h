#pragma once

#include "faust-lib/common/vector.h"

#include "faust-lib/parser/lexer.h"

namespace faust {

enum FaustTokens {
  TK_EOF = 0,
  TK_DECIMAL,
  TK_FLOATING_DECIMAL,
  TK_HEXADECIMAL,
  TK_BINARY,
  TK_STRING,
  TK_LEFT_ROUND_BRACKET,
  TK_RIGHT_ROUND_BRACKET,
  TK_LEFT_SQUARE_BRACKET,
  TK_RIGHT_SQUARE_BRACKET,
  TK_LEFT_CURLY_BRACKET,
  TK_RIGHT_CURLY_BRACKET,
  TK_ASSIGN,
  TK_ARROW,
  TK_COLON,
  TK_SEMICOLON,
  TK_COMMENT,
  TK_MULTILINE_COMMENT,
  TK_FN,
  TK_CONST,
  TK_MUT,
  TK_IMPORT,
  TK_IDENTIFIER,
};

class FaustLexer {
public:
  FaustLexer();
  Vector<Lexer<FaustTokens>::Lexeme> lex(const icu::UnicodeString &text);

private:
  Lexer<FaustTokens> lexer;

protected:
};

} // namespace faust
