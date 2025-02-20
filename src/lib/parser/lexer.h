#pragma once

#include "lib/common/vector.h"
#include "unicode/regex.h"
#include <unicode/utypes.h>

namespace faust {

template <typename TokenEnumeration> class Lexer {
public:
  struct Lexeme {
    TokenEnumeration token;
    icu::UnicodeString value;
  };

  Lexer(const TokenEnumeration &endOfFile) { eofToken = endOfFile; }

  void addToken(const icu::UnicodeString &pattern, const TokenEnumeration &token) {
    UErrorCode status = U_ZERO_ERROR;
    TokenDefinition td;
    td.expression = pattern;
    td.token = token;
    td.matcher = icu::RegexMatcher(td.expression, 0, status);
    tokens.push_back(td);
  }

  void setText(const icu::UnicodeString &newText) {
    UErrorCode status = U_ZERO_ERROR;
    position = 0;
    state = newText;
    icu::UnicodeString disjunctFullExpression = u"(";

    for (size_t i = 0; i < tokens.size(); i++) {
      disjunctFullExpression.append(tokens[i].expression);
      if (i < (tokens.size() - 1)) {
        disjunctFullExpression.append(u"|");
      }
    }

    disjunctFullExpression.append(u")");
    fullMatcher = icu::RegexMatcher(disjunctFullExpression, 0, status);
  }

  void next() { state.tempSubString(position); }

private:
  struct TokenDefinition {
    icu::UnicodeString expression;
    icu::RegexMatcher matcher;
    TokenEnumeration token;
  };
  TokenEnumeration eofToken;
  icu::UnicodeString state;
  icu::RegexMatcher fullMatcher;
  size_t position;
  Vector<TokenDefinition> tokens;
};

} // namespace faust
