cmake_minimum_required(VERSION 3.16)
project(SearchEngine)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
if(MINGW)
    set(gtest_disable_pthreads ON)
endif()
FetchContent_MakeAvailable(googletest)

find_package(Threads REQUIRED)

add_executable(SearchEngine 
    src/main.cpp 
    src/ConverterJSON.cpp
    src/InvertedIndex.cpp
    src/SearchServer.cpp
)
target_include_directories(SearchEngine PUBLIC include)
target_link_libraries(SearchEngine PRIVATE nlohmann_json::nlohmann_json Threads::Threads)


add_executable(search_engine_test
    tests/search_engine_test.cpp
    src/InvertedIndex.cpp
    src/SearchServer.cpp
)

target_include_directories(search_engine_test PUBLIC include)

target_link_libraries(search_engine_test PRIVATE 
    gtest_main 
    Threads::Threads
)

enable_testing()
add_test(NAME Google_Tests COMMAND search_engine_test)