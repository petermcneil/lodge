#include <utility>
#include <iostream>
#include <functional>
#include "VideoFile.h"
#include "Encoder.h"

using namespace std;
using namespace lodge;
namespace log = spdlog;

VideoFile::VideoFile(filesystem::path inputFile, filesystem::path outputFile, SubtitleFile *subtitle) {
    this->inputFilePath = canonical(inputFile.remove_trailing_separator());
    this->outputFilePath = weakly_canonical(outputFile.remove_trailing_separator());
    this->subtitleFile = subtitle;
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
        log::error("Packet not allocated");
        return -1;
    }

    ret = avformat_open_input(&format, this->inputFilePath.c_str(), nullptr, nullptr);

    if (ret != 0) {
        log::error("Couldn't open video file: ", inputFilePath.generic_string());
        return -1;
    }

    ret = avformat_find_stream_info(format, nullptr);

    if (ret < 0) {
        log::error("Wasn't able to generate stream information for file: ", inputFilePath.generic_string());
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
        log::error("This file doesn't contain a video stream: ", inputFilePath.generic_string());
        return -1;
    }

    codec = avcodec_find_decoder(format->streams[videoStream]->codecpar->codec_id);
    parser = av_parser_init(codec->id);

    if (!parser) {
        log::error("Parser doesn't exist for ", codec->id);
        return -1;
    }

    context = avcodec_alloc_context3(codec);
    if (!context) {
        log::error("Could not allocate video codec context");
        return -1;
    }

    ret = avcodec_parameters_to_context(context, format->streams[videoStream]->codecpar);

    if (ret < 0) {
        log::error("Failed to copy codec params to codec context");
        return -1;
    }

    ret = avcodec_open2(context, codec, nullptr);
    if (ret < 0) {
        log::error("Could not open codec");
        return -1;
    }

    f = fopen(inputFilePath.c_str(), "rb");

    if (!f) {
        log::error("Could not open file: ", inputFilePath.generic_string());
        return -1;
    }

    picture = av_frame_alloc();

    if (!picture) {
        log::error("Could not allocate video frame");
        exit(-1);
    }

    while (av_read_frame(format, pkt) >= 0) {
        if (pkt->stream_index == videoStream) {
            ret = decode(pkt, context, picture);
            if (ret < 0) {
                break;
            }
            --framesToSave;
            if (framesToSave <= 0) {
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
    log::info(filename);
    // Open file
    f = fopen(filename, "w");

    if (!f) {
        log::error("Output file failed to open: {}", filename);
        exit(-1);
    }

    log::debug("Saving frame to: {}", filename);
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

void VideoFile::delete_saved_frames() {
    vector<string> all_matching_files;
    const regex frameFilter("frame-*");
    spdlog::debug("Deleting files with path: {} and file: {}", this->outputFilePath.generic_string(),
                  frameFilter.str());

    filesystem::directory_iterator end_itr;
    for (filesystem::directory_iterator i(this->outputFilePath); i != end_itr; ++i) {
        if (!filesystem::is_regular_file(i->status())) continue;

        smatch what;

        if (!boost::regex_search(i->path().filename().string(), what, frameFilter)) continue;
        string path = this->outputFilePath.generic_string() + "/" + i->path().filename().string();
        all_matching_files.push_back(path);
    }

    for (auto &all_matching_file : all_matching_files) {
        spdlog::debug("Removing file {}", all_matching_file);
        filesystem::remove(all_matching_file);
    }
}

int VideoFile::encodeSubtitleFile() {

    return 0;
}

int VideoFile::encodeFrame(AVFrame *frame) {
    vector<bitset<8>> *line = this->subtitleFile->read_next_line();
    int x = frame->width;
    int y = frame->height;

    int p = x * 3 + y * frame->linesize[0];
    auto r = frame->data[0][p];
    auto g = frame->data[0][p + 1];
    auto b = frame->data[0][p + 2];

//    lsb<unsigned char>::write_lsb(r, "");
    return 0;
}

void VideoFile::encode(AVCodecContext *enc_ctx, AVFrame *frame, AVPacket *pkt, FILE *outfile) {
    int ret = 0;

    /* send the frame to the encoder */
    if (frame) {
        log::debug("Send frame {}", frame->pts);

        ret = avcodec_send_frame(enc_ctx, frame);
    }

    if (ret < 0) {
        log::error("Error sending a frame for encoding");
        exit(1);
    }

    while (ret >= 0) {
        ret = avcodec_receive_packet(enc_ctx, pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            return;
        } else if (ret < 0) {
            log::error("Error during encoding");
            exit(1);
        }

        log::debug("Write packet {} (size={})", pkt->pts, pkt->size);
        fwrite(pkt->data, 1, static_cast<size_t>(pkt->size), outfile);
        av_packet_unref(pkt);
    }
}

int VideoFile::bob() {
    const char *codec_name;
    const AVCodec *codec;
    AVCodecContext *c = nullptr;
    int i, ret = 0, x, y;
    FILE *f;
    AVFrame *frame;
    AVPacket *pkt;
    uint8_t endcode[] = {0, 0, 1, 0xb7};

    codec_name = "libx264";

    /* find the mpeg1video encoder */
    codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!codec) {
        log::error("Codec '{}' not found", codec_name);
        exit(1);
    }

    c = avcodec_alloc_context3(codec);
    if (!c) {
        log::error("Could not allocate video codec context");
        exit(1);
    }

    pkt = av_packet_alloc();
    if (!pkt)
        exit(1);

    /* put sample parameters */
    c->bit_rate = 400000;
    /* resolution must be a multiple of two */
    c->width = 352;
    c->height = 288;
    /* frames per second */
    c->time_base = {1, 25};
    c->framerate = {25, 1};

    /* emit one intra frame every ten frames
     * check frame pict_type before passing frame
     * to encoder, if frame->pict_type is AV_PICTURE_TYPE_I
     * then gop_size is ignored and the output of encoder
     * will always be I frame irrespective to gop_size
     */
    c->gop_size = 10;
    c->max_b_frames = 1;
    c->pix_fmt = AV_PIX_FMT_YUV420P;

    if (codec->id == AV_CODEC_ID_H264) {
        av_opt_set(c->priv_data, "preset", "veryslow", 0);
        av_opt_set(c->priv_data, "crf", "0", 0);
    }

    /* open it */

    ret = avcodec_open2(c, codec, nullptr);
    if (ret < 0) {
        log::error("Could not open codec: {}", av_err2str(ret));
        exit(1);
    }

    f = fopen(outputFilePath.c_str(), "wb");
    if (!f) {
        log::error("Could not open {}", this->outputFilePath.c_str());
        exit(1);
    }

    frame = av_frame_alloc();
    if (!frame) {
        log::error("Could not allocate video frame");
        exit(1);
    }
    frame->format = c->pix_fmt;
    frame->width = c->width;
    frame->height = c->height;

    ret = av_frame_get_buffer(frame, 32);
    if (ret < 0) {
        log::error("Could not allocate the video frame data");
        exit(1);
    }

    /* encode 1 second of video */
    for (i = 0; i < 100; i++) {
        fflush(stdout);

        /* make sure the frame data is writable */
        ret = av_frame_make_writable(frame);
        if (ret < 0)
            exit(1);
        /// REPLACE THIS WITH VIDEO FILE PROPER
        /* prepare a dummy image */
        /* Y */
        for (y = 0; y < c->height; y++) {
            for (x = 0; x < c->width; x++) {
                frame->data[0][y * frame->linesize[0] + x] = static_cast<uint8_t>(x + y + i * 3);
            }
        }

        /* Cb and Cr */
        for (y = 0; y < c->height / 2; y++) {
            for (x = 0; x < c->width / 2; x++) {
                frame->data[1][y * frame->linesize[1] + x] = static_cast<uint8_t>(128 + y + i * 2);
                frame->data[2][y * frame->linesize[2] + x] = static_cast<uint8_t>(64 + x + i * 5);
            }
        }

        frame->pts = i;

        /* encode the image */
        encode(c, frame, pkt, f);
    }

    /* flush the encoder */
    encode(c, nullptr, pkt, f);

    /* add sequence end code to have a real MPEG file */
    fwrite(endcode, 1, sizeof(endcode), f);
    fclose(f);

    avcodec_free_context(&c);
    av_frame_free(&frame);
    av_packet_free(&pkt);

    return 0;
}