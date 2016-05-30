cmake_minimum_required (VERSION 3.1)
project (verbly)

find_package(PkgConfig)
pkg_check_modules(sqlite3 sqlite3>=3.8.3 REQUIRED)

include_directories(vendor/json/src)
add_library(verbly lib/data.cpp lib/adjective.cpp lib/noun.cpp lib/verb.cpp lib/adverb.cpp lib/token.cpp lib/word.cpp lib/frame.cpp lib/preposition.cpp lib/adjective_query.cpp lib/adverb_query.cpp lib/noun_query.cpp lib/verb_query.cpp lib/frame_query.cpp)
set_property(TARGET verbly PROPERTY CXX_STANDARD 11)
set_property(TARGET verbly PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(verbly ${sqlite3_LIBRARIES})
