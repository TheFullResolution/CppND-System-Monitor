#ifndef HELPER_PARSER_H
#define HELPER_PARSER_H

#include <fstream>
#include <regex>
#include <string>

#include "linux_parser.h"
#include "parser_consts.h"

using std::string;
using std::vector;

namespace ParserHelper {
template <typename T>
T GetSimpleValue(string const &filter, string const &filename) {
  string line, key;
  T value;

  std::ifstream stream(ParserConsts::kProcDirectory + filename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == filter) {
          return value;
        }
      }
    }
  }
  return value;
};
}  // namespace ParserHelper
#endif