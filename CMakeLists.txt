cmake_minimum_required(VERSION 3.9.1)

project(card)

add_library(sqlparser SHARED IMPORTED)
add_library(bf SHARED IMPORTED)
set_target_properties(sqlparser PROPERTIES IMPORTED_LOCATION /usr/local/lib/libsqlparser.so)
set_target_properties(bf PROPERTIES IMPORTED_LOCATION /usr/local/lib/libbf.so)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_executable(card
    "src/main.cpp"
)

target_link_libraries(card sqlparser bf)
target_include_directories(card PRIVATE "/usr/local/include/hsql" "/usr/local/include/bf")

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()
include(GoogleTest)

add_executable(parser_test test/parser_test.cpp)
target_link_libraries(parser_test GTest::gtest_main sqlparser bf)
target_include_directories(parser_test PRIVATE "/usr/local/include/hsql" "/usr/local/include/bf")
gtest_discover_tests(parser_test)

add_executable(bloom_filter_test test/bloom_filter_test.cpp)
target_link_libraries(bloom_filter_test GTest::gtest_main sqlparser bf)
target_include_directories(bloom_filter_test PRIVATE "/usr/local/include/hsql" "/usr/local/include/bf")
gtest_discover_tests(bloom_filter_test)