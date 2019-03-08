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

#include "SubtitleFile.h"

using namespace std;
using namespace boost;
namespace lodge {

    class VideoFile {
        filesystem::path inputFilePath;
        filesystem::path outputFilePath;
        SubtitleFile *subtitleFile;
        int read_x = 0;
        int read_y = 0;

        int write_x = 0;
        int write_y = 0;

        AVFormatContext *input_format_context;
        AVFormatContext *output_format_context;

        typedef struct FilteringContext {
            AVFilterContext *buffersink_ctx;
            AVFilterContext *buffersrc_ctx;
            AVFilterGraph *filter_graph;
        } FilteringContext;

        FilteringContext *filter_ctx;

        typedef struct StreamContext {
            AVCodecContext *dec_ctx;
            AVCodecContext *enc_ctx;
        } StreamContext;

        StreamContext *stream_ctx;

        AVPacket packet = {.data = nullptr, .size = 0};
        AVFrame *frame = nullptr;
        string input_string =
                R"(Insert this data stream please: !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~)";
        bool run_it_more = true;

    private:
        char read_char_from_frame(AVFrame *fr);

        int write_char_to_frame(AVFrame *fr, bitset<8> bs);

        int encode_write_frame(AVFrame *filt_frame, unsigned int stream_index, int *got_frame);

        int filter_encode_write_frame(AVFrame *frame, unsigned int stream_index);

        int flush_encoder(unsigned int stream_index);

        int end(int ret);

        int open_input_file();

        int open_output_file();

        int init_filter(FilteringContext *fctx, AVCodecContext *dec_ctx,
                        AVCodecContext *enc_ctx, const char *filter_spec);

        int init_filters();

        int perform_steg_frame(AVFrame *fr);

        void write_steg_header(AVFrame *fr);

        int read_steg_header(AVFrame *fr);

    public:
        VideoFile(filesystem::path videoFilePath,
                  filesystem::path outputFilePath,
                  SubtitleFile *subtitleFilePath);

        int write_subtitle_file();

        int read_subtitle_file();

    };

};

#endif
