#include <utility>
#include <iostream>
#include <functional>
#include "VideoFile.h"
#include "Encoder.h"

using namespace std;

lodge::VideoFile::VideoFile(string inputFile, string outputFile) {
    new VideoFile(filesystem::path(inputFile).remove_trailing_separator(),
                  filesystem::path(outputFile).remove_trailing_separator());
}

lodge::VideoFile::VideoFile(filesystem::path inputFile, filesystem::path outputFile) {
    this->inputFilePath = std::move(inputFile.remove_trailing_separator());
    this->outputFilePath = std::move(outputFile.remove_trailing_separator());
}

int lodge::VideoFile::saveFrames(int framesToSave) {
    const AVCodec *codec;
    AVCodecParserContext *parser;
    AVCodecContext *context = nullptr;
    AVFormatContext *format = avformat_alloc_context();
    FILE *f;
    AVFrame *picture;
    int ret;
    AVPacket *pkt;

    pkt = av_packet_alloc();

    if (!pkt) {
        spdlog::error("Packet not allocated");
        return -1;
    }

    ret = avformat_open_input(&format, this->inputFilePath.c_str(), nullptr, nullptr);

    if (ret != 0) {
        spdlog::error("Couldn't open video file: ", inputFilePath.generic_string());
        return -1;
    }

    ret = avformat_find_stream_info(format, nullptr);

    if (ret < 0) {
        spdlog::error("Wasn't able to generate stream information for file: ", inputFilePath.generic_string());
        return -1;
    }

    int videoStream = -1337;

    for (unsigned int i = 0; i < format->nb_streams; i++) {
        auto codecType = format->streams[i]->codecpar->codec_type;

        if (codecType == AVMEDIA_TYPE_VIDEO) {
            videoStream = i;
            break;
        }
    }

    if (videoStream == -1337) {
        spdlog::error("This file doesn't contain a video stream: ", inputFilePath.generic_string());
        return -1;
    }

    codec = avcodec_find_decoder(format->streams[videoStream]->codecpar->codec_id);
    parser = av_parser_init(codec->id);

    if (!parser) {
        spdlog::error("Parser doesn't exist for ", codec->id);
        return -1;
    }

    context = avcodec_alloc_context3(codec);
    if (!context) {
        spdlog::error("Could not allocate video codec context");
        return -1;
    }

    ret = avcodec_parameters_to_context(context, format->streams[videoStream]->codecpar);

    if (ret < 0) {
        spdlog::error("Failed to copy codec params to codec context");
        return -1;
    }
    ret = avcodec_open2(context, codec, nullptr);
    if (ret < 0) {
        spdlog::error("Could not open codec");
        return -1;
    }

    f = fopen(inputFilePath.c_str(), "rb");

    if (!f) {
        spdlog::error("Could not open file: ", inputFilePath.generic_string());
        return -1;
    }

    picture = av_frame_alloc();

    if (!picture) {
        spdlog::error("Could not allocate video frame");
        exit(-1);
    }

    while (av_read_frame(format, pkt) >= 0) {

        if (pkt->stream_index == videoStream) {
            ret = decode(pkt, context, picture);
            if (ret < 0) {
                break;
            }
            if (--framesToSave <= 0) {
                break;
            }
        }
        av_packet_unref(pkt);

    }

    fclose(f);

    av_parser_close(parser);
    avcodec_free_context(&context);
    av_frame_free(&picture);
    av_packet_free(&pkt);

    return 0;
}

int lodge::VideoFile::decode(AVPacket *pkt, AVCodecContext *codecContext, AVFrame *frame) {
    int ret;

    ret = avcodec_send_packet(codecContext, pkt);

    if (ret < 0) {
        spdlog::error("Error sending a packet to the decoder.");
        exit(-1);
    }

    while (ret >= 0) {
        ret = avcodec_receive_frame(codecContext, frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            break;
        } else if (ret < 0) {
            spdlog::error("Error while receiving a frame from the decoder: {}", 0);
            return ret;
        }

        if (ret >= 0) {
            ret = this->savePgmFrame(frame, codecContext);
            av_frame_unref(frame);
        }

    }
    return 0;
}

int lodge::VideoFile::savePgmFrame(AVFrame *frame, AVCodecContext *context) {
    FILE *f;
    int i;
    char filename[1024];
    int xsize = frame->width;
    int ysize = frame->height;
    int wrap = frame->linesize[0];

    snprintf(filename, sizeof(filename), "%s/%s-%d.pgm", this->outputFilePath.c_str(), "frame", context->frame_number);

    // Open file
    f = fopen(filename, "w");

    if (!f) {
        spdlog::error("Output file failed to open: {}", filename);
        exit(-1);
    }

    spdlog::debug("Saving frame to: {}", filename);
    // Write header
    fprintf(f, "P5\n%d %d\n%d\n", xsize, ysize, context->frame_number);

    // Write pixel data
    for (i = 0; i < ysize; i++) {
        fwrite(frame->data[0] + i * wrap, 1, static_cast<size_t>(xsize), f);
    }

    // Close file
    fclose(f);
    return 0;
}

int save_subtitle_file(AVFrame *frame) {
    string f = "Hello, let's insert this";

    return lodge::lsb<unsigned long>::insert_into_frame(frame, f);
}

void lodge::VideoFile::delete_saved_frames() {
    std::vector<std::string> all_matching_files;
    const regex frameFilter("frame-*");
    spdlog::info("Deleting files with path: {} and file: {}", this->outputFilePath.generic_string(), frameFilter.str());

    filesystem::directory_iterator end_itr;
    for (filesystem::directory_iterator i(this->outputFilePath); i != end_itr; ++i) {
        if (!filesystem::is_regular_file(i->status())) continue;

        smatch what;

        if (!boost::regex_search(i->path().filename().string(), what, frameFilter)) continue;

        all_matching_files.push_back(this->outputFilePath.generic_string() + "/" + i->path().filename().string());
    }

    for (auto &all_matching_file : all_matching_files) {
        spdlog::info("Removing file {}", all_matching_file);
        filesystem::remove(all_matching_file);
    }
}

