#pragma once

#include <iostream>
#include <memory>
#include <ostream>
#include <stdexcept>

#include "unicode/regex.h"

#include "faust-lib/common/core.h"
#include "faust-lib/common/vector.h"
#include "unicode/utypes.h"

namespace faust {

constexpr u64 regexEndValue = 0xFFFF'FFFF'FFFF'FFFF;

u64 countNewlines(const icu::UnicodeString &unicodeString);
inline void checkIcuError(const UErrorCode &errorCode, std::string errorInfo) {
  if (U_FAILURE(errorCode)) {
    throw std::logic_error(errorInfo);
  }
}

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
    std::string tokenUtf8Pattern;
    pattern.toUTF8String(tokenUtf8Pattern);
    checkIcuError(status, "Token initialization " + tokenUtf8Pattern);
    tokens.push_back(std::move(td));
  }

  void setText(const icu::UnicodeString &newText) {
    state = newText;
    finished = false;
    position = 0;
    lineNumber = 1;
    for (u64 i = 0; i < tokens.size(); i++) {
      tokens[i].currentStateIndex = 0;
    }
  }

  Lexeme next() {
    UErrorCode status = U_ZERO_ERROR;
    if (position >= state.length()) {
      finished = true;
      return {eofToken, u"", lineNumber};
    }

    u64 currentBegin = 0;
    u64 currentMinimum = 0xFFFF'FFFF'FFFF'FFFF, currentEnd = 0;
    Lexeme currentOutput = {eofToken, u"", 0};

    icu::UnicodeString currentString = state.tempSubString(position);

    bool foundOne = false;
    for (u64 i = 0; i < tokens.size(); i++) {
      std::unique_ptr<icu::RegexMatcher> tokenMatcher =
          std::unique_ptr<icu::RegexMatcher>(tokens[i].pattern->matcher(currentString, status));
      checkIcuError(status, "Token matcher creation");
      if (!tokenMatcher->find()) {
        tokens[i].currentStateIndex = regexEndValue;
        continue;
      }
      foundOne = true;
      u64 start = tokenMatcher->start64(status);
      checkIcuError(status, "Match start retrieval");
      u64 end = tokenMatcher->end64(status);
      checkIcuError(status, "Match end retrieval");
      tokens[i].currentStateIndex = position + start;
      if (tokens[i].currentStateIndex < currentMinimum) {
        currentMinimum = tokens[i].currentStateIndex;
        currentOutput = {tokens[i].token, tokenMatcher->group(status)};
        currentEnd = end;
        currentBegin = start;
      }
    }

    currentOutput.lineNumber = lineNumber + countNewlines(currentString.tempSubString(0, currentBegin));
    lineNumber += countNewlines(currentString.tempSubString(0, currentEnd));

    position += currentEnd;

    if (!foundOne) {
      finished = true;
    }

    return currentOutput;
  }

  bool hasNext() noexcept { return !finished; }

private:
  struct TokenDefinition {
    TokenDefinition(const icu::UnicodeString &expr, std::unique_ptr<icu::RegexPattern> &&pat, TokenEnumeration tok)
        : expression{expr}, pattern{std::move(pat)}, token{tok} {};
    icu::UnicodeString expression;
    std::unique_ptr<icu::RegexPattern> pattern;
    TokenEnumeration token;
    u64 currentStateIndex;
  };
  TokenEnumeration eofToken;
  icu::UnicodeString state;
  int position;
  u64 lineNumber;
  bool finished;
  Vector<TokenDefinition> tokens;
};

} // namespace faust
