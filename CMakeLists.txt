cmake_minimum_required (VERSION 3.1)
project (verbly)

find_package(PkgConfig)
pkg_check_modules(sqlite3 sqlite3>=3.8.3 REQUIRED)

add_library(verbly
  lib/filter.cpp
  lib/field.cpp
  lib/notion.cpp
  lib/word.cpp
  lib/frame.cpp
  lib/part.cpp
  lib/form.cpp
  lib/pronunciation.cpp
  lib/statement.cpp
  lib/database.cpp
  lib/token.cpp)

target_include_directories(verbly PUBLIC
  lib
  vendor/hkutil)

set_property(TARGET verbly PROPERTY CXX_STANDARD 17)
set_property(TARGET verbly PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(verbly ${sqlite3_LIBRARIES})
