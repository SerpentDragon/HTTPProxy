#pragma once

#define __STDC_CONSTANT_MACROS

#include <chrono>
#include <string>
#include <iostream>
#include <boost/asio.hpp>

extern "C"
{
    #include <libavutil/avutil.h>
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavformat/avio.h>
}

namespace asio = boost::asio;
using udp = asio::ip::udp;

int read_data(void*, uint8_t*, int);

std::string generate_filename();


class ffmpeg_manager
{
public:

    ffmpeg_manager(const std::string&);

    ffmpeg_manager(const ffmpeg_manager&) = delete;

    ffmpeg_manager& operator=(const ffmpeg_manager&) = delete;

    ffmpeg_manager(ffmpeg_manager&&) noexcept = default;

    ffmpeg_manager& operator=(ffmpeg_manager&&) noexcept = default;

    ~ffmpeg_manager();

public:

    bool init_input_ctx(udp::socket&);

    void write_data();

private:

    void close_output_file();

    bool open_output_file(const std::string&);

    void free_resources();

private:

    AVFormatContext* input_ctx_;
    AVFormatContext* output_ctx_;

    int buffer_size_;
    uint8_t* buffer_;

    std::string output_dir_;
};
