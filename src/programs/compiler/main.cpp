#include "lib/parser/lexer.h"

enum TestTokens { TK_FN, TK_DECIMAL_NUMBER, TK_STRING };

int main() { faust::Lexer<TestTokens> lex; }
