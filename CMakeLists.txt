cmake_minimum_required(VERSION 3.10)

project(Proxy VERSION 1.0.0)

set(SOURCE_FILE
    src/main.cpp
    src/logger.cpp
)

file(COPY src DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(proxy ${SOURCE_FILE})

# target_link_libraries(proxy PRIVATE Boost::system Boost::thread Boost::program_options)

set_target_properties(proxy PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)