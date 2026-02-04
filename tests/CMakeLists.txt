add_executable(search_engine_test
    search_engine_test.cpp
    ../src/InvertedIndex.cpp
    ../src/SearchServer.cpp
)

target_include_directories(search_engine_test PRIVATE ../include)

target_link_libraries(search_engine_test PRIVATE 
    gtest_main 
    Threads::Threads
)

enable_testing()
add_test(NAME Google_Tests COMMAND search_engine_test)