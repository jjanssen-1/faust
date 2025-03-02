#pragma once

#include <memory>
#include <ostream>
#include <stdexcept>

#include "unicode/regex.h"

#include "faust-lib/common/core.h"
#include "faust-lib/common/vector.h"

namespace faust {

u64 countNewlines(const icu::UnicodeString &unicodeString);

template <typename TokenEnumeration> class Lexer {
public:
  struct Lexeme {
    TokenEnumeration token;
    icu::UnicodeString value;
    u64 lineNumber;
    friend std::ostream &operator<<(std::ostream &stream, const Lexeme &lexeme) {
      std::string utf8String;
      lexeme.value.toUTF8String(utf8String);
      return stream << "{TokenId: " << static_cast<u32>(lexeme.token) << ", Value: " << utf8String
                    << ", LineNumber: " << lexeme.lineNumber << "}";
    }
  };

  Lexer(const TokenEnumeration &endOfFile) { eofToken = endOfFile; }

  void addToken(const icu::UnicodeString &pattern, const TokenEnumeration &token) {
    UErrorCode status = U_ZERO_ERROR;
    TokenDefinition td(
        pattern, std::move(std::unique_ptr<icu::RegexPattern>(icu::RegexPattern::compile(pattern, 0, status))), token);
    tokens.push_back(std::move(td));
  }

  void setText(const icu::UnicodeString &newText) {
    UErrorCode status = U_ZERO_ERROR;
    position = 0;
    state = newText;
    icu::UnicodeString disjunctFullExpression = u"(";

    for (size_t i = 0; i < tokens.size(); i++) {
      disjunctFullExpression.append(tokens[i].expression);
      if (i < (tokens.size() - 1)) {
        disjunctFullExpression.append(u")|(");
      }
    }

    disjunctFullExpression.append(u")");
    fullPattern = std::unique_ptr<icu::RegexPattern>(icu::RegexPattern::compile(disjunctFullExpression, 0, status));
    finished = false;
  }

  Lexeme next() {
    if (position >= state.length()) {
      finished = true;
      return {eofToken, u"", lineNumber};
    }

    u64 lineNum = 0;

    UErrorCode status = U_ZERO_ERROR;
    icu::UnicodeString currentString = state.tempSubString(position);
    fullMatcher = std::unique_ptr<icu::RegexMatcher>(fullPattern->matcher(currentString, status));

    if (fullMatcher->find()) {
      u64 start = fullMatcher->start64(status);
      u64 end = fullMatcher->end64(status);

      icu::UnicodeString match = fullMatcher->group(status);
      position += end;

      lineNum = countNewlines(currentString.tempSubString(0, start)) + lineNumber;
      lineNumber += countNewlines(currentString.tempSubString(0, end));

      for (const TokenDefinition &tokDef : tokens) {
        // Todo precalculate tokenMatcher per run.
        std::unique_ptr<icu::RegexMatcher> tokTester(tokDef.pattern->matcher(match, status));
        if (tokTester->find()) {
          return {tokDef.token, match, 0};
        }
      }
      // If TokenDefinitions didn't match something could be wrong with the icu library.
      throw std::logic_error("Found token has to match subgroup token.");
    }

    finished = true;
    return {eofToken, u"", lineNum};
  }

  bool hasNext() noexcept { return !finished; }

private:
  struct TokenDefinition {
    TokenDefinition(const icu::UnicodeString &expr, std::unique_ptr<icu::RegexPattern> &&pat, TokenEnumeration tok)
        : expression{expr}, pattern{std::move(pat)}, token{tok} {};
    icu::UnicodeString expression;
    std::unique_ptr<icu::RegexPattern> pattern;
    TokenEnumeration token;
  };
  TokenEnumeration eofToken;
  icu::UnicodeString state;
  std::unique_ptr<icu::RegexPattern> fullPattern;
  std::unique_ptr<icu::RegexMatcher> fullMatcher;
  int position;
  u64 lineNumber;
  bool finished;
  Vector<TokenDefinition> tokens;
};

} // namespace faust
