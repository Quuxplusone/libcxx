//===----------------------------------------------------------------------===//
//
//                     the llvm compiler infrastructure
//
// this file is dual licensed under the mit and the university of illinois open
// source licenses. see license.txt for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03, c++11, c++14, c++17

// <regex>

// template <class traits, class charT, class FST>
//     basic_string<charT>
//     regex_replace(const charT* s,
//                   const basic_regex<charT, traits>& e,
//                   basic_string_view<charT, FST> fmt,
//                   regex_constants::match_flag_type flags =
//                                              regex_constants::match_default);

#include <regex>
#include <cassert>
#include "test_macros.h"

int main() {
  {
     std::regex phone_numbers("\\d{3}-\\d{4}");
     const char* phone_book("555-1234, 555-2345, 555-3456");
     std::string r = std::regex_replace(phone_book,
                                                                 phone_numbers,
																 std::string_view("123-$&"),
																 std::regex_constants::match_default);
     assert(r == "123-555-1234, 123-555-2345, 123-555-3456");
  }
  {
    std::regex phone_numbers("\\d{3}-\\d{4}");
    const char* phone_book("555-1234, 555-2345, 555-3456");
    std::string r = std::regex_replace(phone_book,
                                                                phone_numbers,
																std::string_view("123-$&"),
																std::regex_constants::format_sed);
    assert(r == "123-$555-1234, 123-$555-2345, 123-$555-3456");
  }
  {
    std::regex phone_numbers("\\d{3}-\\d{4}");
    const char* phone_book("555-1234, 555-2345, 555-3456");
    std::string r = std::regex_replace(phone_book,
                                                                phone_numbers,
																std::string_view("123-&"),
																std::regex_constants::format_sed);
    assert(r == "123-555-1234, 123-555-2345, 123-555-3456");
  }
  {
    std::regex phone_numbers("\\d{3}-\\d{4}");
    const char* phone_book("555-1234, 555-2345, 555-3456");
    std::string r = std::regex_replace(phone_book,
                                                                phone_numbers,
																std::string_view("123-$&"),
																std::regex_constants::format_no_copy);
    assert(r == "123-555-1234123-555-2345123-555-3456");
  }
  {
    std::regex phone_numbers("\\d{3}-\\d{4}");
    const char* phone_book("555-1234, 555-2345, 555-3456");
    std::string r = std::regex_replace(phone_book,
                                                                phone_numbers,
																std::string_view("123-$&"),
																std::regex_constants::format_first_only);
    assert(r == "123-555-1234, 555-2345, 555-3456");
  }
  {
    std::regex phone_numbers("\\d{3}-\\d{4}");
    const char* phone_book("555-1234, 555-2345, 555-3456");
    std::string r = std::regex_replace(phone_book,
                                                                phone_numbers,
																std::string_view("123-$&"),
																std::regex_constants::format_first_only | std::regex_constants::format_no_copy);
    assert(r == "123-555-1234");
  }
}
