/*=============================================================================

    Example program showing use of RE2C with iterators

    Copyright (c) 2017 Jeffrey E Trull. Distributed under the Boost
    Software License, Version 1.0. (See accompanying file
    LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <cstring>
#include <iostream>
#include <boost/container/slist.hpp>

#include "lexer.hpp"    // (generated by RE2C from lexer.re)
#include "padded_range.hpp"

int main() {
    // strategy 1: copy-and-pad

    // five good tokens
    auto all_good_toks = "alephalphaaardvarka";
    std::string all_good_str(all_good_toks);
    all_good_str += std::string(YYMAXFILL, 'x');    // padding
    auto beg = all_good_str.begin();
    // iterate until we get an error
    while (auto opt_str = lex(beg, all_good_str.end())) {
        std::cout << *opt_str << "\n";
    }

    std::cout << "----\n";

    // intersperse some error input
    auto some_bad_toks = "alealpha";
    // "a" first (we only partially spelled "aleph")
    // then two errors (the incomplete "l" and "e"), then "alpha"

    std::string some_bad_str(some_bad_toks);
    some_bad_str += std::string(YYMAXFILL, 'x');
    beg = some_bad_str.begin();
    // iterate until input is exhausted (since "no data" and "wrong data" both return empty)
    while ( std::distance(beg, some_bad_str.end()) > YYMAXFILL ) {
        auto opt_str = lex(beg, some_bad_str.end());
        if (opt_str) {
            std::cout << *opt_str << "\n";
        } else {
            std::cout << "ERROR\n";
        }
    }
    std::cout << "----\n";

    // strategy 2: use a special padded iterator (no copy, but overhead on each access)
    using padded_range_t = padded_range<char const*, 'x', YYMAXFILL>;

    auto prng = padded_range_t(all_good_toks, all_good_toks + std::strlen(all_good_toks));
    auto prbeg = std::begin(prng);
    while (auto opt_str = lex(prbeg, std::end(prng))) {
        std::cout << *opt_str << "\n";
    }
    std::cout << "----\n";

    // and the version with errors
    auto prng_errs = padded_range_t(some_bad_toks, some_bad_toks + std::strlen(some_bad_toks));
    prbeg = std::begin(prng_errs);
    while ( std::distance(prbeg, std::end(prng_errs)) > YYMAXFILL ) {
        auto opt_str = lex(prbeg, std::end(prng_errs));
        if (opt_str) {
            std::cout << *opt_str << "\n";
        } else {
            std::cout << "ERROR\n";
        }
    }
    std::cout << "----\n";

    // you can also pad forward iterators (e.g. this singly linked list from Boost)
    using padded_list_t =
        padded_range<boost::container::slist<char>::const_iterator, 'x', YYMAXFILL>;

    boost::container::slist<char>
        all_good_list(all_good_toks, all_good_toks + std::strlen(all_good_toks));
    auto plng = padded_list_t(all_good_list.begin(), all_good_list.end());
    auto plbeg = std::begin(plng);
    while (auto opt_str = lex(plbeg, std::end(plng))) {
        std::cout << *opt_str << "\n";
    }
    std::cout << "----\n";

    boost::container::slist<char>
        some_bad_list(some_bad_toks, some_bad_toks + std::strlen(some_bad_toks));
    auto plng_errs = padded_list_t(some_bad_list.begin(), some_bad_list.end());
    plbeg = std::begin(plng_errs);
    while ( std::distance(plbeg, std::end(plng_errs)) > YYMAXFILL ) {
        auto opt_str = lex(plbeg, std::end(plng_errs));
        if (opt_str) {
            std::cout << *opt_str << "\n";
        } else {
            std::cout << "ERROR\n";
        }
    }

}
