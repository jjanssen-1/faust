#include <fstream>
#include <iostream>
#include <stdexcept>

#include "faust-lib/parser/faustLexer.h"

void readUtf8FileToUnicodeString(const std::string &filePath, icu::UnicodeString &unicodeString) {
  std::ifstream file(filePath, std::ios::binary);
  if (!file.is_open()) {
    throw std::runtime_error("File could not be opened");
  }

  std::string utf8Content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  file.close();

  unicodeString = icu::UnicodeString::fromUTF8(utf8Content);
}

int main(int argc, char **argv) {
  try {
    icu::UnicodeString fileContent;
    faust::FaustLexer lex;

    if (argc != 2) {
      throw std::runtime_error("Faustc requires one argument (file path)");
    }

    std::cout << "Trying to read " << argv[1] << std::endl;
    readUtf8FileToUnicodeString(argv[1], fileContent);

    auto lexemes = lex.lex(fileContent);
    for (auto lexeme : lexemes) {
      std::cout << "FoundToken: " << lexeme << std::endl;
    }
  } catch (const std::runtime_error &e) {
    std::cerr << "\n\nCaught exception: " << e.what() << std::endl;
  }

  return 0;
}
