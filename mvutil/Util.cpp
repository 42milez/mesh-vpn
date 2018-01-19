//
// Created by Akihiro TAKASE on 2017/12/03.
//

#include "Util.h"

namespace mvutil {

  // convert string into hex
  // https://stackoverflow.com/questions/3381614/c-convert-string-to-hexadecimal-and-vice-versa
  std::string string_to_hex(const std::string &input) {
    static const char *const lut = "0123456789ABCDEF";
    size_t len = input.length();
    std::string output;
    output.reserve(2 * len);

    for (size_t i = 0; i < len; i++) {
      auto c = input[i];
      output.push_back(lut[(c >> 4) & 0x0F]);
      output.push_back(lut[c & 15]);
    }

    return output;
  }

  // convert hex into string
  // https://stackoverflow.com/questions/3381614/c-convert-string-to-hexadecimal-and-vice-versa
  std::string hex_to_string(const std::string &input) {
    static const char *const lut = "0123456789ABCDEF";
    size_t len = input.length();

    if (len & 1) throw std::invalid_argument("odd length");

    std::string output;
    output.reserve(len / 2);

    for (size_t i = 0; i < len; i += 2) {
      char a = input[i];
      const char *p = std::lower_bound(lut, lut + 16, a);
      if (*p != a) throw std::invalid_argument("not a hex digit");
      char b = input[i + 1];
      const char *q = std::lower_bound(lut, lut + 16, b);
      if (*q != b) throw std::invalid_argument("not a hex digit");
      output.push_back(((p - lut) << 4) | (q - lut));
    }

    return output;
  }

}
