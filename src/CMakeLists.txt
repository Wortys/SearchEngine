add_executable(SearchEngine 
    main.cpp
    ConverterJSON.cpp
    InvertedIndex.cpp
    SearchServer.cpp
)

target_include_directories(SearchEngine PRIVATE ../include)

target_link_libraries(SearchEngine PRIVATE 
    nlohmann_json::nlohmann_json
    Threads::Threads
)