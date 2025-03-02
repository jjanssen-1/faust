#include <lexer.h>

faust::u64 countNewlines(const icu::UnicodeString &unicodeString) {
  constexpr faust::u16 LF = 0x000A, CR = 0x000D;
  bool lastCr = false;
  faust::u64 count = 0;
  for (int i = 0; i < unicodeString.length(); i++) {
    UChar32 character = unicodeString.char32At(i);
    if (character == LF && !lastCr) { // LF only (UNIX)
      count++;
    }
    lastCr = false;
    if (character == CR) { // CR (Mac and Windows)
      count++;
      lastCr = true;
    }
  }
  return count;
}
