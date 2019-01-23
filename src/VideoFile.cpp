#include <utility>
#include <iostream>
#include "VideoFile.h"

using namespace std;

VideoFile::VideoFile(string inputFile, string outputFile) {
    this->inputFilePath = filesystem::path(inputFile).remove_trailing_separator();
    this->outputFilePath = filesystem::path(outputFile).remove_trailing_separator();
}

VideoFile::VideoFile(filesystem::path inputFile, filesystem::path outputFile) {
    this->inputFilePath = std::move(inputFile.remove_trailing_separator());
    this->outputFilePath = std::move(outputFile.remove_trailing_separator());
}

int VideoFile::saveFrames(int framesToSave) {
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
        cout << "Packet not allocated" << endl;
        std::system("pause");
        return -1;
    }

    ret = avformat_open_input(&format, this->inputFilePath.c_str(), nullptr, nullptr);

    if (ret != 0) {
        cout << "Couldn't open video file: " << inputFilePath.generic_string() << endl;
        return -1;
    }

    ret = avformat_find_stream_info(format, nullptr);

    if (ret < 0) {
        cout << "Wasn't able to generate stream information for file: " << inputFilePath.generic_string() << endl;
        return -1;
    }


    int videoStream = -1337;

    for (int i = 0; i < format->nb_streams; i++) {
        auto codecType = format->streams[i]->codecpar->codec_type;

        if (codecType == AVMEDIA_TYPE_VIDEO) {
            videoStream = i;
            break;
        }
    }

    if (videoStream == -1337) {
        cout << "This file doesn't contain a video stream: " << inputFilePath.generic_string() << endl;
        return -1;
    }

    codec = avcodec_find_decoder(format->streams[videoStream]->codecpar->codec_id);
    parser = av_parser_init(codec->id);

    if (!parser) {
        cout << "Parser doesn't exist for " << codec->id << endl;
        return -1;
    }

    context = avcodec_alloc_context3(codec);
    if (!context) {
        fprintf(stderr, "Could not allocate video codec context\n");
        return -1;
    }

    ret = avcodec_parameters_to_context(context, format->streams[videoStream]->codecpar);

    if (ret < 0) {
        cout << "Failed to copy codec params to codec context" << endl;
        return -1;
    }
    ret = avcodec_open2(context, codec, nullptr);
    if (ret < 0) {
        cout << "Could not open codec" << endl;
        return -1;
    }

    f = fopen(inputFilePath.c_str(), "rb");

    if (!f) {
        cout << "Could not open file: " << inputFilePath.generic_string() << endl;
        return -1;
    }

    picture = av_frame_alloc();

    if (!picture) {
        fprintf(stderr, "Could not allocate video frame\n");
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

int VideoFile::decode(AVPacket *pkt, AVCodecContext *codecContext, AVFrame *frame) {
    char buf[1024];
    int ret;

    ret = avcodec_send_packet(codecContext, pkt);

    if (ret < 0) {
        cout << "Error sending a packet to the decoder." << endl;
        exit(-1);
    }

    while (ret >= 0) {
        ret = avcodec_receive_frame(codecContext, frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            break;
        } else if (ret < 0) {
            cout << "Error while receiving a frame from the decoder: " << ret << endl;
            return ret;
        }

        if (ret >= 0) {
            snprintf(buf, sizeof(buf), "%s/%s-%d.pgm", outputFilePath.c_str(), "frame", codecContext->frame_number);
            this->savePgmFrame(frame->data[0], frame->linesize[0], frame->width, frame->height, buf);
            av_frame_unref(frame);
        }

    }
    return 0;
}

void VideoFile::savePgmFrame(unsigned char *buf, int wrap, int xsize, int ysize, char *filename) {
    FILE *f;
    int i;

    // Open file
    f = fopen(filename, "w");

    if (!f) {
        cout << "Output file failed to open: " << filename << endl;
        exit(-1);
    }

    cout << "Saving frame to: " << filename << endl;
    // Write header
    fprintf(f, "P5\n%d %d\n%d\n", xsize, ysize, 255);

    // Write pixel data
    for (i = 0; i < ysize; i++) {
        fwrite(buf + i * wrap, 1, static_cast<size_t>(xsize), f);
    }

    // Close file
    fclose(f);
}
