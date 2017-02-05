cmake_minimum_required (VERSION 3.1)
project (generator)

find_package(PkgConfig)
pkg_check_modules(sqlite3 sqlite3 REQUIRED)
find_package(libxml2 REQUIRED)

include_directories(${sqlite3_INCLUDE_DIR} ${LIBXML2_INCLUDE_DIR})
add_executable(generator notion.cpp word.cpp lemma.cpp form.cpp pronunciation.cpp group.cpp frame.cpp part.cpp database.cpp field.cpp generator.cpp main.cpp)
set_property(TARGET generator PROPERTY CXX_STANDARD 11)
set_property(TARGET generator PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(generator ${sqlite3_LIBRARIES} ${LIBXML2_LIBRARIES})
