#include "faustLexer.h"

namespace faust {

FaustLexer::FaustLexer() : lexer(TK_EOF) {
  lexer.addToken(u"\\d+\\.\\d+", TK_FLOATING_DECIMAL);
  lexer.addToken(u"0x[0-9A-Fa-f]+", TK_HEXADECIMAL);
  lexer.addToken(u"0b[01]+", TK_BINARY);
  lexer.addToken(u"\\d+", TK_DECIMAL); // Order of operations important
  lexer.addToken(u"([\"'])(?:\\\\\\1|.)*?\\1", TK_STRING);
  lexer.addToken(u"\\(", TK_LEFT_ROUND_BRACKET);
  lexer.addToken(u"\\)", TK_RIGHT_ROUND_BRACKET);
  lexer.addToken(u"\\[", TK_LEFT_SQUARE_BRACKET);
  lexer.addToken(u"\\]", TK_RIGHT_SQUARE_BRACKET);
  lexer.addToken(u"\\{", TK_LEFT_CURLY_BRACKET);
  lexer.addToken(u"\\}", TK_RIGHT_CURLY_BRACKET);
  lexer.addToken(u"\\=", TK_ASSIGN);
  lexer.addToken(u"->", TK_ARROW);
  lexer.addToken(u":", TK_COLON);
  lexer.addToken(u";", TK_SEMICOLON);
  lexer.addToken(u"\\/\\/[^\\r\\n]*", TK_COMMENT);
  lexer.addToken(u"\\/\\*[\\s\\S]*?\\*\\/", TK_MULTILINE_COMMENT);
  lexer.addToken(u"\\bfn\\b", TK_FN);
  lexer.addToken(u"\\bconst\\b", TK_CONST);
  lexer.addToken(u"\\bmut\\b", TK_MUT);
  lexer.addToken(u"\\bimport\\b", TK_IMPORT);
  lexer.addToken(u"\\b[a-zA-Z][a-zA-Z0-9_]*\\b", TK_IDENTIFIER);
}

Vector<Lexer<FaustTokens>::Lexeme> FaustLexer::lex(const icu::UnicodeString &text) {
  lexer.setText(text);
  Vector<Lexer<FaustTokens>::Lexeme> lexemes;
  while (lexer.hasNext()) {
    Lexer<FaustTokens>::Lexeme next = lexer.next();
    lexemes.push_back(next);
  }
  return lexemes;
}

} // namespace faust
