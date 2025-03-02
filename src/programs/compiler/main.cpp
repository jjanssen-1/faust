#include <iostream>
#include <vector>

#include "faust-lib/parser/lexer.h"

enum TestTokens { TK_EOF, TK_FN, TK_DECIMAL_NUMBER, TK_STRING };

int main() {
  faust::Lexer<TestTokens> lex(TK_EOF);
  lex.addToken(u"fn", TK_FN);
  lex.addToken(u"\\d+\\.\\d+", TK_DECIMAL_NUMBER);
  lex.addToken(u"\"[a-zA-Z]*\"", TK_STRING);

  lex.setText(u"fn 1.0 \"hello\"");

  std::vector<faust::Lexer<TestTokens>::Lexeme> lexemes;

  while (lex.hasNext()) {
    faust::Lexer<TestTokens>::Lexeme next = lex.next();
    lexemes.push_back(next);
    std::cout << next << std::endl;
  }

  return 0;
}
