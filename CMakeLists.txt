#    Build control file for RE2C with iterators example
#
#    Copyright (C) 2017 Jeff Trull <edaskel@att.net>
#
#    Distributed under the Boost Software License, Version 1.0. (See accompanying
#    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#

cmake_minimum_required( VERSION 3.6 )

find_package( Boost 1.65.1 REQUIRED )

# to supply your own path use -DRE2C=... on the CMake command line
find_program( RE2C NAMES re2c DOC "Path to re2c executable" )

if( NOT RE2C )
  message( FATAL_ERROR "No re2c executable found" )
endif()

# use re2c to generate lexer.hpp from lexer.re
add_custom_command(
  OUTPUT lexer.hpp
  COMMAND ${RE2C} -b -o ${CMAKE_CURRENT_BINARY_DIR}/lexer.hpp ${CMAKE_CURRENT_SOURCE_DIR}/lexer.re
  MAIN_DEPENDENCY lexer.re
)

add_executable( ex example.cpp )
target_include_directories( ex PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ) # to pick up the generated lexer
target_sources( ex PRIVATE lexer.hpp )
target_link_libraries( ex Boost::boost )
set_property( TARGET ex PROPERTY CXX_STANDARD 17 )
