cmake_minimum_required(VERSION 3.9.1)

project(card)

add_library(sqlparser SHARED IMPORTED)
set_target_properties(sqlparser PROPERTIES IMPORTED_LOCATION /usr/local/lib/libsqlparser.so)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_executable(card
    "src/main.cpp"
)

target_link_libraries(card sqlparser)
target_include_directories(card PRIVATE "/usr/local/include/hsql")