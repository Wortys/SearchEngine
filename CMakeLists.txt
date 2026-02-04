cmake_minimum_required(VERSION 3.16)
project(SearchEngine)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/refs/heads/main.zip)
if(MINGW)
    set(gtest_disable_pthreads ON)
endif()
FetchContent_MakeAvailable(googletest)

find_package(Threads REQUIRED)

add_subdirectory(src)
add_subdirectory(tests)