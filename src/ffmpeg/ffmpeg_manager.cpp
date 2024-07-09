#include "ffmpeg_manager.h"

int read_data(void* opaque, uint8_t* buf, int size)
{
    boost::system::error_code error;
    udp::socket* socket = reinterpret_cast<udp::socket*>(opaque);

    std::size_t len = socket->receive(asio::buffer(buf, size), 0, error);

    return len ? len : -1;
}

std::string generate_filename()
{
    auto now = std::chrono::system_clock::now();
	return std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count()) + ".ts";
}


ffmpeg_manager::ffmpeg_manager(const std::string& output_dir) 
    : input_ctx_(nullptr), output_ctx_(nullptr), buffer_size_(65536), 
    buffer_(static_cast<uint8_t*>(av_malloc(buffer_size_))), output_dir_(output_dir + '/')
{
    avformat_network_init();
}

ffmpeg_manager::~ffmpeg_manager()
{
    free_resources();
}

bool ffmpeg_manager::init_input_ctx(udp::socket& socket)
{
    AVIOContext* avio_ctx = avio_alloc_context(buffer_, buffer_size_, 0, &socket, read_data, nullptr, nullptr);
    if (!avio_ctx)
    {
        std::cerr << "Error create avio context!\n";
        return false;
    }

    input_ctx_ = avformat_alloc_context();
    if (!input_ctx_)
    {
        std::cerr << "Error create input context!\n";
        avio_context_free(&avio_ctx);
        return false;
    }
    input_ctx_->pb = avio_ctx;
    input_ctx_->flags = AVFMT_FLAG_CUSTOM_IO;

    if (avformat_open_input(&input_ctx_, nullptr, nullptr, nullptr) < 0)
    {
        std::cerr << "Could not open input context" << std::endl;
        return false;
    }

    if (avformat_find_stream_info(input_ctx_, nullptr) < 0)
    {
        std::cerr << "Could not find stream information" << std::endl;
        avformat_close_input(&input_ctx_);
        return false;
    }

    return true;
}

void ffmpeg_manager::write_data()
{
    int segment_index = 0;
    int64_t segment_duration = 60 * AV_TIME_BASE;
    int64_t current_segment_start_time = 0;

    std::string output_filename = output_dir_ + generate_filename();

    if (!open_output_file(output_filename)) 
    {
        avformat_close_input(&input_ctx_);
        return;
    }

    AVPacket packet;
    while (av_read_frame(input_ctx_, &packet) >= 0) 
    {
        AVStream* in_stream = input_ctx_->streams[packet.stream_index];
        AVStream* out_stream = output_ctx_->streams[packet.stream_index];

        int64_t packet_pts_time = av_rescale_q(packet.pts, in_stream->time_base, {1, AV_TIME_BASE});
        if (packet_pts_time - current_segment_start_time >= segment_duration) 
        {
            close_output_file();

            current_segment_start_time = packet_pts_time;

            output_filename = output_dir_ + generate_filename();
            if (!open_output_file(output_filename)) 
            {
                avformat_close_input(&input_ctx_);
                return;
            }
        }

        packet.pts = av_rescale_q_rnd(packet.pts, in_stream->time_base, out_stream->time_base, AV_ROUND_NEAR_INF);
        packet.dts = av_rescale_q_rnd(packet.dts, in_stream->time_base, out_stream->time_base, AV_ROUND_NEAR_INF);
        packet.duration = av_rescale_q(packet.duration, in_stream->time_base, out_stream->time_base);
        packet.pos = -1;

        if (av_interleaved_write_frame(output_ctx_, &packet) < 0) 
        {
            std::cerr << "Error muxing packet" << std::endl;
            break;
        }

        av_packet_unref(&packet);
    }

    close_output_file();
    avformat_close_input(&input_ctx_);
}

void ffmpeg_manager::close_output_file()
{
    if (output_ctx_) 
    {
        av_write_trailer(output_ctx_);

        if (!(output_ctx_->oformat->flags & AVFMT_NOFILE)) 
        {
            avio_closep(&output_ctx_->pb);
        }
        
        avformat_free_context(output_ctx_);
        output_ctx_ = nullptr;
    }
}

bool ffmpeg_manager::open_output_file(const std::string& output_filename) 
{
    const AVOutputFormat* output_format = av_guess_format("mpegts", nullptr, nullptr);
    if (!output_format) 
    {
        std::cerr << "Could not guess output format" << std::endl;
        return false;
    }

    if (avformat_alloc_output_context2(&output_ctx_, output_format, nullptr, output_filename.c_str()) < 0) 
    {
        std::cerr << "Could not create output context" << std::endl;
        return false;
    }

    for (unsigned int i = 0; i < input_ctx_->nb_streams; i++) 
    {
        AVStream* in_stream = input_ctx_->streams[i];
        AVStream* out_stream = avformat_new_stream(output_ctx_, nullptr);
        if (!out_stream) 
        {
            std::cerr << "Failed to allocate output stream" << std::endl;
            return false;
        }

        if (avcodec_parameters_copy(out_stream->codecpar, in_stream->codecpar) < 0) 
        {
            std::cerr << "Failed to copy codec parameters" << std::endl;
            return false;
        }

        out_stream->codecpar->codec_tag = 0;
    }

    if (!(output_ctx_->oformat->flags & AVFMT_NOFILE)) 
    {
        if (avio_open(&output_ctx_->pb, output_filename.c_str(), AVIO_FLAG_WRITE) < 0) 
        {
            std::cerr << "Could not open output file: " << output_filename << std::endl;
            return false;
        }
    }

    if (avformat_write_header(output_ctx_, nullptr) < 0) 
    {
        std::cerr << "Error occurred when writing header to output file" << std::endl;
        return false;
    }

    return true;
}

void ffmpeg_manager::free_resources()
{
    av_free(buffer_);

    if (input_ctx_)
    {
        avformat_close_input(&input_ctx_);
    }

    if (output_ctx_)
    {
        close_output_file();
    }

    if (input_ctx_ && input_ctx_->pb)
    {
        avio_context_free(&input_ctx_->pb);
    }
}
