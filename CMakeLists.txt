cmake_minimum_required (VERSION 3.1)
project (verbly)

find_package(PkgConfig)
pkg_check_modules(sqlite3 sqlite3>=3.8.3 REQUIRED)

set(CMAKE_BUILD_TYPE Debug)

include_directories(vendor/json)
add_library(verbly lib/filter.cpp lib/field.cpp lib/notion.cpp lib/word.cpp lib/group.cpp lib/frame.cpp lib/lemma.cpp lib/form.cpp lib/pronunciation.cpp lib/statement.cpp lib/binding.cpp lib/database.cpp)
set_property(TARGET verbly PROPERTY CXX_STANDARD 11)
set_property(TARGET verbly PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(verbly ${sqlite3_LIBRARIES})
