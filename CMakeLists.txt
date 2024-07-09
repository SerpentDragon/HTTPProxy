cmake_minimum_required(VERSION 3.10)

project(Proxy VERSION 1.0.0)

include_directories(src)

set(SOURCE_FILE
    src/main.cpp
    src/server/http_server.cpp
    src/server/http_session.cpp
    src/server/udp_connection.cpp
    src/logger/logger.cpp
    src/ffmpeg/ffmpeg_manager.cpp
)

set(HEADERS
    src/server/http_server.h
    src/server/http_session.h
    src/server/udp_connection.h
    src/logger/logger.h
    src/ffmpeg/ffmpeg_manager.h
)

add_executable(proxy ${SOURCE_FILE} ${HEADERS})
add_executable(client src/client/client.cpp)

find_package(Boost COMPONENTS system thread program_options url REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(AVFORMAT libavformat REQUIRED)
pkg_check_modules(AVCODEC libavcodec REQUIRED)
pkg_check_modules(AVUTIL libavutil REQUIRED)

target_link_libraries(proxy PRIVATE Boost::system Boost::thread Boost::program_options Boost::url
                            ${AVFORMAT_LIBRARIES} ${AVCODEC_LIBRARIES} ${AVUTIL_LIBRARIES})
target_link_libraries(client PRIVATE Boost::system Boost::thread Boost::program_options Boost::url)

set_target_properties(proxy PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
