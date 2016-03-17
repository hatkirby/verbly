cmake_minimum_required (VERSION 2.6)
project (verbly)

find_package(PkgConfig)
pkg_check_modules(sqlite3 sqlite3 REQUIRED)

add_library(verbly lib/data.cpp lib/adjective.cpp lib/noun.cpp lib/verb.cpp lib/adverb.cpp lib/token.cpp lib/word.cpp)
set_property(TARGET verbly PROPERTY CXX_STANDARD 11)
set_property(TARGET verbly PROPERTY CXX_STANDARD_REQUIRED ON)
