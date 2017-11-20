#    Build control file for RE2C with iterators unit tests
#
#    Copyright (C) 2017 Jeff Trull <edaskel@att.net>
#
#    Distributed under the Boost Software License, Version 1.0. (See accompanying
#    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#

cmake_minimum_required( VERSION 3.6 )

# Get gtest
include( ExternalProject )
ExternalProject_Add( gtest
  GIT_REPOSITORY https://github.com/google/googletest.git
  INSTALL_COMMAND ""  # do not install or update
  UPDATE_COMMAND ""
)
ExternalProject_Get_Property( gtest SOURCE_DIR )
set( GTEST_SOURCE_DIR ${SOURCE_DIR} )
ExternalProject_Get_Property( gtest BINARY_DIR )
set( GTEST_BINARY_DIR ${BINARY_DIR} )

# kind of a hack - cmake doesn't like it when INTERFACE_INCLUDE_DIRECTORIES does not exist
# (though it is OK with IMPORTED_LOCATION not being there yet...)
file( MAKE_DIRECTORY ${GTEST_SOURCE_DIR}/googletest/include )

add_library( gtest_lib STATIC IMPORTED )
add_dependencies( gtest_lib gtest )
set_target_properties( gtest_lib PROPERTIES
  IMPORTED_LOCATION ${GTEST_BINARY_DIR}/googlemock/gtest/libgtest.a
  INTERFACE_INCLUDE_DIRECTORIES ${GTEST_SOURCE_DIR}/googletest/include
)
add_library( gtest_main STATIC IMPORTED )
add_dependencies( gtest_main gtest )
set_target_properties( gtest_main PROPERTIES
  IMPORTED_LOCATION ${GTEST_BINARY_DIR}/googlemock/gtest/libgtest_main.a
)

# Unit tests for the padded iterator

add_executable( basic basic.cpp )
target_include_directories( basic
  PUBLIC ${CMAKE_SOURCE_DIR}
)
set_property( TARGET basic PROPERTY CXX_STANDARD 17 )
target_link_libraries( basic gtest_lib pthread gtest_main )

add_test( basic basic )
