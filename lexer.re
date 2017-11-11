// A simple lexer example that requires lookahead

// command line: re2c -b --input custom -o fill_example_lexer.hpp fill_example_lexer.re

// ensure YYMAXFILL is generated
/*!max:re2c*/

#include <string>
#include <optional>

template<typename Iter>
std::optional<std::string>
lex(Iter       & YYCURSOR,        // current lexing position
    Iter const & YYLIMIT)         // end of input, including padding
{
   using YYCTYPE = typename std::iterator_traits<Iter>::value_type;
   Iter YYMARKER = YYCURSOR;

   auto token = YYCURSOR;   // remember starting point

   // we disable the YYFILL mechanism, padding the input as required instead
   /*!re2c
       re2c:yyfill:enable = 0;

       "a" | "alpha" | "aleph" | "aardvark" {
           return std::string(token, YYCURSOR);
       }
       * { return std::optional<std::string>(); }
   */
}
