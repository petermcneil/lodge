#ifndef LODGE_VIDEOFILE_H
#define LODGE_VIDEOFILE_H

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/avutil.h>
#include <libavutil/opt.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/opt.h>
#include <libavutil/pixdesc.h>
#include <libavutil/log.h>
};

#include <string>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <spdlog/spdlog.h>

#include "subtitle.h"
#include "frame_header.h"

using namespace std;
using namespace boost;

namespace lodge {

    class video {
    private:
        filesystem::path inputFilePath;
        filesystem::path outputFilePath;
        subtitle *subtitle_file;
        vector<frame_header> *headers = new vector<frame_header>;

        int read_x = 0;
        int read_y = 0;

        int write_x = 0;
        int write_y = 0;
        int block_size = 1;
        int no_of_frames = 0;
        int no_of_bits_in_char = 8;

        //Keeps a line persisted over multiple frames
        vector<bitset<8>> *write_line_bs = new vector<bitset<8>>;

        AVFormatContext *input_format_context{};
        AVFormatContext *output_format_context{};

        typedef struct FilteringContext {
            AVFilterContext *buffersink_ctx;
            AVFilterContext *buffersrc_ctx;
            AVFilterGraph *filter_graph;
        } FilteringContext;

        FilteringContext *filter_ctx{};

        typedef struct StreamContext {
            AVCodecContext *dec_ctx;
            AVCodecContext *enc_ctx;
        } StreamContext;

        StreamContext *stream_ctx{};

        AVPacket packet = {.data = nullptr, .size = 0};
        AVFrame *frame = nullptr;
        bool checked_header = false;
        vector<char> character_vector;


        //READ
        string output;
        const AVCodec *codec;
        AVCodecParserContext *parser;
        AVCodecContext *context;
        AVFormatContext *format;
        FILE *f;
        AVFrame *picture;
        int ret;
        AVPacket *pkt;
        int video_stream;

        char read_char_from_frame(AVFrame *fr);

        int *write_char_to_frame(AVFrame *fr, bitset<8> bs);

        int encode_write_frame(AVFrame *filt_frame, unsigned int stream_index, int *got_frame);

        int filter_encode_write_frame(AVFrame *fr, unsigned int stream_index);

        int flush_encoder(unsigned int stream_index);

        int end(int retu);

        int open_input_file();

        int open_output_file();

        int init_filter(FilteringContext *fctx, AVCodecContext *dec_ctx,
                        AVCodecContext *enc_ctx, const char *filter_spec);

        int init_filters();

        int perform_steg_frame(AVFrame *fr);

        void write_steg_header(AVFrame *fr, frame_header h);

        frame_header *read_steg_header(AVFrame *fr);

        int generate_frame_headers(AVFrame *fr);

        int init_read();

    public:
        video(string inputVideo, subtitle *subtitlefile);

        video(string videoFilePath, string outputFilePath, subtitle *subtitleFile);

        int write_subtitle_file();

        int read_subtitle_file();

        bool has_steg_file();

    };

};

#endif
