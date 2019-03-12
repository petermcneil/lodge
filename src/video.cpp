#include <utility>
#include <iostream>
#include <functional>
#include "video.h"
#include "encoder.h"

using namespace std;
using namespace lodge;
namespace log = spdlog;

video::video(string inputVideo, string outputVideo, subtitle *subFile) {
    this->inputFilePath = canonical(filesystem::path(inputVideo));
    this->outputFilePath = weakly_canonical(filesystem::path(outputVideo));
    this->subtitleFile = subFile;
}

video::video(string inputVideo, subtitle *subFile) {
    this->inputFilePath = canonical(filesystem::path(inputVideo));
    this->subtitleFile = subFile;
}

video::video(filesystem::path inputFile, filesystem::path outputFile, subtitle *subtitleFile) {
    this->inputFilePath = canonical(inputFile.remove_trailing_separator());
    this->outputFilePath = weakly_canonical(outputFile.remove_trailing_separator());
    this->subtitleFile = subtitleFile;
}

int video::write_char_to_frame(AVFrame *fr, bitset<8> bs) {
//    log::debug("Writing char '{}' at pos '{}'", subtitle::bin_to_char(bs), this->write_x);
    for (int iter = 0; iter < 8; ++iter) {
        unsigned char b;
        auto bit = bs[iter];
        if (bit) {
            b = static_cast<unsigned char>(1);
        } else {
            b = static_cast<unsigned char>(0);
        }
        auto pos = fr->linesize[0] * this->write_y + this->write_x;
        auto rChannel = fr->data[0][pos];
        lsb<unsigned char>::write_lsb(rChannel, b);
        fr->data[0][pos] = rChannel;
        ++this->write_x;
    }

    return 0;
}

char video::read_char_from_frame(AVFrame *f) {
    bitset<8> bs = bitset<8>();
    for (size_t iter = 0; iter < 8; ++iter) {
        auto pos = f->linesize[0] * this->read_y + this->read_x;
        auto bit = lsb<unsigned char>::read_lsb(f->data[0][pos]);
        bs.set(iter, bit);
        ++this->read_x;
    }
    return subtitle::bin_to_char(bs);
}

int video::perform_steg_frame(AVFrame *fr) {
    frame_header *h = this->subtitleFile->header;
    cout << "Writing a frame header" << endl;
    cout << h->to_string() << endl;
    this->write_steg_header(fr, h);

    cout << "======================================="<< endl;
    cout << "\tWriting lines to frame 0" << endl;
    while (this->subtitleFile->has_next_line()) {
        auto line = this->subtitleFile->read_next_line();
        for (auto character : *line) {
            this->write_char_to_frame(fr, character);
        }
    }
    cout << "=======================================" << endl;
    return 0;
}

void video::write_steg_header(AVFrame *f, frame_header *h) {
    string h_string = h->to_string();
    log::debug("Writing frame_header: '{}", h_string);
    for (char &ch : h_string) {
        bitset<8> converted = subtitle::char_to_bin(ch);
        this->write_char_to_frame(f, converted);
    }
}

frame_header *video::read_steg_header(AVFrame *f) {
    log::debug("Reading steg_header from a frame");
    string found_header;

    int count = 0;
    while (true) {
        if (found_header.empty()) {
            char c = this->read_char_from_frame(f);
            found_header += c;
        } else {
            if (regex_match(found_header, frame_header::header_regex)) {
                break;
            } else if (count == 6) {
                log::debug("Found frame_header '{}' should be |LODGE|", found_header);
                assert(found_header == "|LODGE|");
            } else if (count >= 99) {
                //Should never reach here as we should always call has_steg_header first
                log::error("File does not contain Lodge steg data: {}", found_header);
                return nullptr;
            }

            char c = this->read_char_from_frame(f);
            found_header += c;
            if (count < 7) {
                --count;
            }
        }

    }

    return new frame_header(found_header);
}

int video::end(int ret) {
    av_packet_unref(&packet);
    av_frame_free(&frame);
    for (unsigned int i = 0; i < input_format_context->nb_streams; i++) {
        avcodec_free_context(&stream_ctx[i].dec_ctx);
        if (output_format_context && output_format_context->nb_streams > i && output_format_context->streams[i] &&
            stream_ctx[i].enc_ctx)
            avcodec_free_context(&stream_ctx[i].enc_ctx);
        if (filter_ctx && filter_ctx[i].filter_graph)
            avfilter_graph_free(&filter_ctx[i].filter_graph);
    }
    av_free(filter_ctx);
    av_free(stream_ctx);
    avformat_close_input(&input_format_context);
    if (output_format_context && !(output_format_context->oformat->flags & AVFMT_NOFILE))
        avio_closep(&output_format_context->pb);
    avformat_free_context(output_format_context);

    if (ret < 0) {
        log::error("Error occurred: {}", av_err2str(ret));
    }

    return ret ? 1 : 0;
}

/**
 * Opens the input file and sets up the stream context for use later
 */
int video::open_input_file() {
    int ret;
    unsigned int iter;

    //Input format set to null
    input_format_context = nullptr;

    //Open input file
    ret = avformat_open_input(&input_format_context, this->inputFilePath.c_str(), nullptr, nullptr);
    if (ret < 0) {
        log::error("Cannot open input file");
        return ret;
    }

    //Get the stream information for the video
    ret = avformat_find_stream_info(input_format_context, nullptr);
    if (ret < 0) {
        log::error("Cannot find stream information");
        return ret;
    }

    //Allocate memory for the stream context
    stream_ctx = (StreamContext *) av_mallocz_array(input_format_context->nb_streams, sizeof(*stream_ctx));
    if (!stream_ctx) {
        return AVERROR(ENOMEM);
    }

    //For every stream in the video, set them up. Be it video or audio.
    for (iter = 0; iter < input_format_context->nb_streams; iter++) {
        //Get the stream
        AVStream *stream = input_format_context->streams[iter];
        //Find the decoder
        AVCodec *dec = avcodec_find_decoder(stream->codecpar->codec_id);
        //Declare a codec context
        AVCodecContext *codec_ctx;
        if (!dec) {
            log::error("Failed to find decoder for stream #{}", iter);
            return AVERROR_DECODER_NOT_FOUND;
        }

        //Allocate memory for the codec_ctx
        codec_ctx = avcodec_alloc_context3(dec);
        if (!codec_ctx) {
            log::error("Failed to allocate the decoder context for stream #{}", iter);
            return AVERROR(ENOMEM);
        }

        //Generate the codec context from the streams parameters
        ret = avcodec_parameters_to_context(codec_ctx, stream->codecpar);
        if (ret < 0) {
            log::error("Failed to copy decoder parameters to input decoder context for stream #{}", iter);
            return ret;
        }

        /* Reencode video & audio and remux subtitles etc. */
        if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO || codec_ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
            if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO) {
                codec_ctx->framerate = av_guess_frame_rate(input_format_context, stream, nullptr);
            }
            /* Open decoder */
            ret = avcodec_open2(codec_ctx, dec, nullptr);
            if (ret < 0) {
                log::error("Failed to open decoder for stream #{}", iter);
                return ret;
            }
        }
        //Set the decode context to be the codec_ctx creadted.
        stream_ctx[iter].dec_ctx = codec_ctx;
    }

    //Print out the important information regarding the input file
    av_dump_format(input_format_context, 0, this->inputFilePath.c_str(), 0);
    return 0;
}

/**
 Opens the output file and sets up empty streams to be written to.
 */
int video::open_output_file() {
    AVStream *out_stream;
    AVStream *in_stream;
    AVCodecContext *decoder_context;
    AVCodecContext *encoder_context;
    AVCodec *encoder;
    int ret;
    unsigned int iter;

    //Allocate memory for the output format context
    output_format_context = nullptr;
    avformat_alloc_output_context2(&output_format_context, nullptr, nullptr, this->outputFilePath.c_str());
    if (!output_format_context) {
        log::error("Could not create output context");
        return AVERROR_UNKNOWN;
    }

    //For every stream inside the input_format_context, build an output stream inside the output_format_context
    for (iter = 0; iter < input_format_context->nb_streams; iter++) {
        out_stream = avformat_new_stream(output_format_context, nullptr);
        if (!out_stream) {
            log::error("Failed allocating output stream");
            return AVERROR_UNKNOWN;
        }

        in_stream = input_format_context->streams[iter];
        decoder_context = stream_ctx[iter].dec_ctx;

        if (decoder_context->codec_type == AVMEDIA_TYPE_VIDEO || decoder_context->codec_type == AVMEDIA_TYPE_AUDIO) {
            encoder = avcodec_find_encoder(decoder_context->codec_id);

            if (!encoder) {
                log::error("Necessary encoder not found");
                return AVERROR_INVALIDDATA;
            }
            encoder_context = avcodec_alloc_context3(encoder);
            if (!encoder_context) {
                log::error("Failed to allocate the encoder context");
                return AVERROR(ENOMEM);
            }

            /* In this example, we transcode to same properties (picture size,
             * sample rate etc.). These properties can be changed for output
             * streams easily using filters */
            if (decoder_context->codec_type == AVMEDIA_TYPE_VIDEO) {
                encoder_context->height = decoder_context->height;
                encoder_context->width = decoder_context->width;
                encoder_context->sample_aspect_ratio = decoder_context->sample_aspect_ratio;
                encoder_context->pix_fmt = decoder_context->pix_fmt;
                /* video time_base can be set to whatever is handy and supported by encoder */
                encoder_context->time_base = av_inv_q(decoder_context->framerate);

                //THIS ENSURES NO COMPRESSION
                av_opt_set(encoder_context->priv_data, "crf", "0", 0);
            } else {
                encoder_context->sample_rate = decoder_context->sample_rate;
                encoder_context->channel_layout = decoder_context->channel_layout;
                encoder_context->channels = av_get_channel_layout_nb_channels(encoder_context->channel_layout);
                /* take first format from list of supported formats */
                encoder_context->sample_fmt = encoder->sample_fmts[0];
                encoder_context->pix_fmt = decoder_context->pix_fmt;
                encoder_context->time_base = (AVRational) {1, encoder_context->sample_rate};
            }

            if (output_format_context->oformat->flags & AVFMT_GLOBALHEADER) {
                encoder_context->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
            }

            /* Third parameter can be used to pass settings to encoder */
            ret = avcodec_open2(encoder_context, encoder, nullptr);
            if (ret < 0) {
                log::error("Cannot open video encoder for stream #{}", iter);
                return ret;
            }
            ret = avcodec_parameters_from_context(out_stream->codecpar, encoder_context);
            if (ret < 0) {
                log::error("Failed to copy encoder parameters to output stream #{}", iter);
                return ret;
            }

            out_stream->time_base = encoder_context->time_base;
            stream_ctx[iter].enc_ctx = encoder_context;
        } else if (decoder_context->codec_type == AVMEDIA_TYPE_UNKNOWN) {
            log::error("Elementary stream #{} is of unknown type, cannot proceed", iter);
            return AVERROR_INVALIDDATA;
        } else {
            /* if this stream must be remuxed */
            ret = avcodec_parameters_copy(out_stream->codecpar, in_stream->codecpar);
            if (ret < 0) {
                log::error("Copying parameters for stream #{} failed", iter);
                return ret;
            }
            out_stream->time_base = in_stream->time_base;
        }
    }

    //Print out output file format data.
    av_dump_format(output_format_context, 0, this->outputFilePath.c_str(), 1);

    if (!(output_format_context->oformat->flags & AVFMT_NOFILE)) {
        ret = avio_open(&output_format_context->pb, this->outputFilePath.c_str(), AVIO_FLAG_WRITE);
        if (ret < 0) {
            log::error("Could not open output file '{}'", this->outputFilePath.c_str());
            return ret;
        }
    }

    /* init muxer, write output file frame_header */
    ret = avformat_write_header(output_format_context, nullptr);
    if (ret < 0) {
        log::error("Error occurred when opening output file");
        return ret;
    }

    return 0;
}

int video::init_filter(FilteringContext *fctx, AVCodecContext *dec_ctx,
                       AVCodecContext *enc_ctx, const char *filter_spec) {
    char args[512];
    int ret = 0;
    const AVFilter *buffersrc = nullptr;
    const AVFilter *buffersink = nullptr;
    AVFilterContext *buffersrc_ctx = nullptr;
    AVFilterContext *buffersink_ctx = nullptr;
    AVFilterInOut *outputs = avfilter_inout_alloc();
    AVFilterInOut *inputs = avfilter_inout_alloc();
    AVFilterGraph *filter_graph = avfilter_graph_alloc();

    if (!outputs || !inputs || !filter_graph) {
        ret = AVERROR(ENOMEM);
        return ret;
    }

    if (dec_ctx->codec_type == AVMEDIA_TYPE_VIDEO) {
        buffersrc = avfilter_get_by_name("buffer");
        buffersink = avfilter_get_by_name("buffersink");
        if (!buffersrc || !buffersink) {
            log::error("filtering source or sink element not found");
            ret = AVERROR_UNKNOWN;
            return ret;
        }

        snprintf(args, sizeof(args),
                 "video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d",
                 dec_ctx->width, dec_ctx->height, dec_ctx->pix_fmt,
                 dec_ctx->time_base.num, dec_ctx->time_base.den,
                 dec_ctx->sample_aspect_ratio.num,
                 dec_ctx->sample_aspect_ratio.den);

        ret = avfilter_graph_create_filter(&buffersrc_ctx, buffersrc, "in",
                                           args, nullptr, filter_graph);
        if (ret < 0) {
            log::error("Cannot create buffer source");
            return ret;
        }

        ret = avfilter_graph_create_filter(&buffersink_ctx, buffersink, "out",
                                           nullptr, nullptr, filter_graph);
        if (ret < 0) {
            log::error("Cannot create buffer sink");
            return ret;
        }

        ret = av_opt_set_bin(buffersink_ctx, "pix_fmts",
                             (uint8_t *) &enc_ctx->pix_fmt, sizeof(enc_ctx->pix_fmt),
                             AV_OPT_SEARCH_CHILDREN);
        if (ret < 0) {
            log::error("Cannot set output pixel format");
            return ret;
        }
    } else if (dec_ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
        buffersrc = avfilter_get_by_name("abuffer");
        buffersink = avfilter_get_by_name("abuffersink");
        if (!buffersrc || !buffersink) {
            log::error("filtering source or sink element not found\n");
            ret = AVERROR_UNKNOWN;
            return ret;
        }

        if (!dec_ctx->channel_layout) {
            dec_ctx->channel_layout = static_cast<uint64_t>(av_get_default_channel_layout(dec_ctx->channels));
        }

        snprintf(args, sizeof(args),
                 "time_base=%d/%d:sample_rate=%d:sample_fmt=%s:channel_layout=0x%"
                 PRIx64,
                 dec_ctx->time_base.num, dec_ctx->time_base.den, dec_ctx->sample_rate,
                 av_get_sample_fmt_name(dec_ctx->sample_fmt),
                 dec_ctx->channel_layout);
        ret = avfilter_graph_create_filter(&buffersrc_ctx, buffersrc, "in",
                                           args, nullptr, filter_graph);
        if (ret < 0) {
            log::error("Cannot create audio buffer source\n");
            return ret;
        }

        ret = avfilter_graph_create_filter(&buffersink_ctx, buffersink, "out",
                                           nullptr, nullptr, filter_graph);
        if (ret < 0) {
            log::error("Cannot create audio buffer sink");
            return ret;
        }

        ret = av_opt_set_bin(buffersink_ctx, "sample_fmts",
                             (uint8_t *) &enc_ctx->sample_fmt, sizeof(enc_ctx->sample_fmt),
                             AV_OPT_SEARCH_CHILDREN);
        if (ret < 0) {
            log::error("Cannot set output sample format");
            return ret;
        }

        ret = av_opt_set_bin(buffersink_ctx, "channel_layouts",
                             (uint8_t *) &enc_ctx->channel_layout,
                             sizeof(enc_ctx->channel_layout), AV_OPT_SEARCH_CHILDREN);
        if (ret < 0) {
            log::error("Cannot set output channel layout");
            return ret;
        }

        ret = av_opt_set_bin(buffersink_ctx, "sample_rates",
                             (uint8_t *) &enc_ctx->sample_rate, sizeof(enc_ctx->sample_rate),
                             AV_OPT_SEARCH_CHILDREN);
        if (ret < 0) {
            log::error("Cannot set output sample rate");
            return ret;
        }
    } else {
        ret = AVERROR_UNKNOWN;
        return ret;
    }

    /* Endpoints for the filter graph. */
    outputs->name = av_strdup("in");
    outputs->filter_ctx = buffersrc_ctx;
    outputs->pad_idx = 0;
    outputs->next = nullptr;

    inputs->name = av_strdup("out");
    inputs->filter_ctx = buffersink_ctx;
    inputs->pad_idx = 0;
    inputs->next = nullptr;

    if (!outputs->name || !inputs->name) {
        ret = AVERROR(ENOMEM);
        return ret;
    }

    if ((ret = avfilter_graph_parse_ptr(filter_graph, filter_spec, &inputs, &outputs, nullptr)) < 0) {
        return ret;
    }


    if ((ret = avfilter_graph_config(filter_graph, nullptr)) < 0) {
        return ret;
    }

    /* Fill FilteringContext */
    fctx->buffersrc_ctx = buffersrc_ctx;
    fctx->buffersink_ctx = buffersink_ctx;
    fctx->filter_graph = filter_graph;

    return ret;
}

int video::init_filters() {
    const char *filter_spec;
    unsigned int i;
    int ret;
    filter_ctx = (FilteringContext *) av_malloc_array(input_format_context->nb_streams, sizeof(*filter_ctx));

    if (!filter_ctx) {
        return AVERROR(ENOMEM);
    }

    for (i = 0; i < input_format_context->nb_streams; i++) {
        filter_ctx[i].buffersrc_ctx = nullptr;
        filter_ctx[i].buffersink_ctx = nullptr;
        filter_ctx[i].filter_graph = nullptr;
        if (!(input_format_context->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO
              || input_format_context->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO))
            continue;


        if (input_format_context->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            filter_spec = "null"; /* passthrough (dummy) filter for video */
        } else {
            filter_spec = "anull"; /* passthrough (dummy) filter for audio */
        }
        ret = init_filter(&filter_ctx[i], stream_ctx[i].dec_ctx,
                          stream_ctx[i].enc_ctx, filter_spec);
        if (ret) {
            return ret;
        }
    }
    return 0;
}

int video::encode_write_frame(AVFrame *filt_frame, unsigned int stream_index, int *got_frame) {
    int ret;
    int got_frame_local;
    AVPacket enc_pkt;
    int (*enc_func)(AVCodecContext *, AVPacket *, const AVFrame *, int *) =
    (input_format_context->streams[stream_index]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) ? avcodec_encode_video2
                                                                                              : avcodec_encode_audio2;
    if (!got_frame) {
        got_frame = &got_frame_local;
    }

    if (this->run_it_more) {
        this->perform_steg_frame(filt_frame);
        run_it_more = false;
    }

    /* encode filtered frame */
    enc_pkt.data = nullptr;
    enc_pkt.size = 0;
    av_init_packet(&enc_pkt);
    ret = enc_func(stream_ctx[stream_index].enc_ctx, &enc_pkt,
                   filt_frame, got_frame);

    av_frame_free(&filt_frame);
    if (ret < 0) {
        return ret;
    }
    if (!(*got_frame)) {
        return 0;
    }

    /* prepare packet for muxing */
    enc_pkt.stream_index = stream_index;
    av_packet_rescale_ts(&enc_pkt,
                         stream_ctx[stream_index].enc_ctx->time_base,
                         output_format_context->streams[stream_index]->time_base);

    /* mux encoded frame */
    ret = av_interleaved_write_frame(output_format_context, &enc_pkt);
    return ret;
}

int video::filter_encode_write_frame(AVFrame *frame, unsigned int stream_index) {
    int ret;
    AVFrame *filt_frame;

//    log::debug("Pushing decoded frame to filters");
    /* push the decoded frame into the filtergraph */
    ret = av_buffersrc_add_frame_flags(filter_ctx[stream_index].buffersrc_ctx,
                                       frame, 0);
    if (ret < 0) {
        log::error("Error while feeding the filtergraph");
        return ret;
    }

    /* pull filtered frames from the filtergraph */
    while (true) {
        filt_frame = av_frame_alloc();
        if (!filt_frame) {
            ret = AVERROR(ENOMEM);
            break;
        }
//        log::debug("Pulling filtered frame from filters");
        ret = av_buffersink_get_frame(filter_ctx[stream_index].buffersink_ctx,
                                      filt_frame);
        if (ret < 0) {
            /* if no more frames for output - returns AVERROR(EAGAIN)
             * if flushed and no more frames for output - returns AVERROR_EOF
             * rewrite retcode to 0 to show it as normal procedure completion
             */
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                ret = 0;
            }
            av_frame_free(&filt_frame);
            break;
        }

        filt_frame->pict_type = AV_PICTURE_TYPE_NONE;
        ret = encode_write_frame(filt_frame, stream_index, nullptr);
        if (ret < 0) {
            break;
        }
    }

    return ret;
}

int video::flush_encoder(unsigned int stream_index) {
    int ret;
    int got_frame;

    if (!(stream_ctx[stream_index].enc_ctx->codec->capabilities & AV_CODEC_CAP_DELAY)) {
        return 0;
    }

    while (true) {
//        log::debug("Flushing stream #{} encoder", stream_index);
        ret = encode_write_frame(nullptr, stream_index, &got_frame);
        if (ret < 0) {
            break;
        }
        if (!got_frame) {
            return 0;
        }
    }
    return ret;
}

int video::write_subtitle_file() {
    av_log_set_level(AV_LOG_QUIET);
    int ret;
    enum AVMediaType type;
    unsigned int stream_index;
    unsigned int i;
    int got_frame;
    int (*dec_func)(AVCodecContext *, AVFrame *, int *, const AVPacket *);

    ret = open_input_file();
    if (ret < 0) {
        log::error("Cannot open input file");
    }

    ret = open_output_file();

    if (ret < 0) {
        log::error("Cannot open output file");
    }

    ret = init_filters();
    if (ret < 0) {
        log::error("Couldn't init filters");
    }

    /* read all packets */
    while (true) {
        if ((ret = av_read_frame(input_format_context, &packet)) < 0) {
            break;
        }

        stream_index = static_cast<unsigned int>(packet.stream_index);
        type = input_format_context->streams[packet.stream_index]->codecpar->codec_type;
//        log::debug("Demuxer gave frame of stream_index {}", stream_index);

        if (filter_ctx[stream_index].filter_graph) {
//            log::debug("Going to re-encode & filter the frame");
            frame = av_frame_alloc();
            if (!frame) {
                ret = AVERROR(ENOMEM);
                break;
            }
            av_packet_rescale_ts(&packet,
                                 input_format_context->streams[stream_index]->time_base,
                                 stream_ctx[stream_index].dec_ctx->time_base);
            dec_func = (type == AVMEDIA_TYPE_VIDEO) ? avcodec_decode_video2 : avcodec_decode_audio4;
            ret = dec_func(stream_ctx[stream_index].dec_ctx, frame,
                           &got_frame, &packet);
            if (ret < 0) {
                av_frame_free(&frame);
                log::error("Decoding failed");
                break;
            }

            if (got_frame) {
                frame->pts = frame->best_effort_timestamp;
                ret = filter_encode_write_frame(frame, stream_index);
                av_frame_free(&frame);
                if (ret < 0) {
                    return end(ret);
                }
            } else {
                av_frame_free(&frame);
            }
        } else {
            /* remux this frame without reencoding */
            av_packet_rescale_ts(&packet,
                                 this->input_format_context->streams[stream_index]->time_base,
                                 this->output_format_context->streams[stream_index]->time_base);

            ret = av_interleaved_write_frame(this->output_format_context, &packet);
            if (ret < 0) {
                return end(ret);
            }
        }
        av_packet_unref(&packet);
    }

    /* flush filters and encoders */
    for (i = 0; i < input_format_context->nb_streams; i++) {
        /* flush filter */
        if (!filter_ctx[i].filter_graph)
            continue;
        ret = filter_encode_write_frame(nullptr, i);
        if (ret < 0) {
            log::error("Flushing filter failed");
            return end(ret);
        }

        /* flush encoder */
        ret = flush_encoder(i);
        if (ret < 0) {
            log::error("Flushing encoder failed");
            return end(ret);
        }
    }

    av_write_trailer(output_format_context);

    return end(ret);
}

int video::read_subtitle_file() {
    if (!checked_header) {
        log::error("Must call has_steg_file() first");
        return -234;
    }
    string output;
    const AVCodec *codec;
    AVCodecParserContext *parser;
    AVCodecContext *context = nullptr;
    AVFormatContext *format = avformat_alloc_context();
    FILE *f;
    AVFrame *picture;
    int ret;
    AVPacket *pkt;

    log::debug("Allocating packet on startup");
    pkt = av_packet_alloc();

    if (!pkt) {
        log::error("Packet not allocated");
        return -1;
    }

    log::debug("Opening input '{}' in libav", this->inputFilePath.generic_string());
    ret = avformat_open_input(&format, this->inputFilePath.c_str(), nullptr, nullptr);

    if (ret != 0) {
        cout << outputFilePath.generic_string() << endl;
        log::error("Couldn't open video file: {}", inputFilePath.generic_string());
        return -1;
    }

    log::debug("Finding stream information about the file");
    ret = avformat_find_stream_info(format, nullptr);

    if (ret < 0) {
        log::error("Wasn't able to generate stream information for file: {} ", inputFilePath.generic_string());
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

    log::debug("Finding the decoder for the video file");
    codec = avcodec_find_decoder(format->streams[videoStream]->codecpar->codec_id);
    log::debug("Initalising a parser with the codec: {}", codec->name);
    parser = av_parser_init(codec->id);

    if (!parser) {
        log::error("Parser doesn't exist for ", codec->id);
        return -1;
    }

    log::debug("Allocating memory for the video codex context");
    context = avcodec_alloc_context3(codec);
    if (!context) {
        log::error("Could not allocate video codec context");
        return -1;
    }

    log::debug("Convert parameters to context");
    ret = avcodec_parameters_to_context(context, format->streams[videoStream]->codecpar);

    if (ret < 0) {
        log::error("Failed to copy codec params to codec context");
        return -1;
    }

    log::debug("Open the codec");
    ret = avcodec_open2(context, codec, nullptr);
    if (ret < 0) {
        log::error("Could not open codec");
        return -1;
    }

    log::debug("Open the input file: '{}'", inputFilePath.generic_string());
    f = fopen(inputFilePath.c_str(), "rb");

    if (!f) {
        log::error("Could not open file: ", inputFilePath.generic_string());
        return -1;
    }

    log::debug("Allocate memory for the frames");
    picture = av_frame_alloc();

    if (!picture) {
        log::error("Could not allocate video frame");
        exit(-1);
    }

    log::debug("Reset read positions to 0");
    this->read_x = 0;
    this->read_y = 0;

    while (av_read_frame(format, pkt) >= 0) {
        if (pkt->stream_index == videoStream) {
            ret = avcodec_send_packet(context, pkt);

            if (ret < 0) {
                log::error("Error sending a packet to the decoder.");
                exit(-1);
            }

            while (ret >= 0) {
                ret = avcodec_receive_frame(context, picture);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                    log::info("Failed to receive frame from packet");
                    break;
                }
                if (ret >= 0) {
                    log::debug("Dealing with the frame");
                    if (this->subtitleFile->header != nullptr) {
                        vector<char> o;
                        auto length = this->subtitleFile->header->size;
                        for (; length > 0; --length) {
                            char c = this->read_char_from_frame(picture);
                            if (c == '\n') {
                                this->subtitleFile->write_line(o);
                                o.clear();
                            } else {
                                o.push_back(c);
                            }
                        }
                    } else {
                        log::error("Frame header is null for some reason");
                    }
                    av_frame_unref(frame);
                    ret = -1;
                }
            }
            break;

        }
        av_packet_unref(pkt);
        break;
    }
    fclose(f);

    av_parser_close(parser);
    avcodec_free_context(&context);
    av_frame_free(&picture);
    av_packet_free(&pkt);

    return 0;
}

bool video::has_steg_file() {
    string output;
    const AVCodec *codec;
    AVCodecParserContext *parser;
    AVCodecContext *context = nullptr;
    AVFormatContext *format = avformat_alloc_context();
    FILE *f;
    AVFrame *picture;
    int ret;
    AVPacket *pkt;

    log::debug("Allocating packet on startup");
    pkt = av_packet_alloc();

    if (!pkt) {
        log::error("Packet not allocated");
        return -1;
    }

    log::debug("Opening input '{}' in libav", this->inputFilePath.generic_string());
    ret = avformat_open_input(&format, this->inputFilePath.c_str(), nullptr, nullptr);

    if (ret != 0) {
        cout << "Couldn't open the video file: " << outputFilePath.generic_string() << endl;
        log::error("Couldn't open video file: {}", inputFilePath.generic_string());
        return -1;
    }

    log::debug("Finding stream information about the file");
    ret = avformat_find_stream_info(format, nullptr);

    if (ret < 0) {
        log::error("Wasn't able to generate stream information for file: {} ", inputFilePath.generic_string());
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

    log::debug("Finding the decoder for the video file");
    codec = avcodec_find_decoder(format->streams[videoStream]->codecpar->codec_id);
    log::debug("Initialising a parser with the codec: {}", codec->name);
    parser = av_parser_init(codec->id);

    if (!parser) {
        log::error("Parser doesn't exist for ", codec->id);
        return -1;
    }

    log::debug("Allocating memory for the video codex context");
    context = avcodec_alloc_context3(codec);
    if (!context) {
        log::error("Could not allocate video codec context");
        return -1;
    }

    log::debug("Convert parameters to context");
    ret = avcodec_parameters_to_context(context, format->streams[videoStream]->codecpar);

    if (ret < 0) {
        log::error("Failed to copy codec params to codec context");
        return -1;
    }

    log::debug("Open the codec");
    ret = avcodec_open2(context, codec, nullptr);
    if (ret < 0) {
        log::error("Could not open codec");
        return -1;
    }

    log::debug("Open the input file: '{}'", inputFilePath.generic_string());
    f = fopen(inputFilePath.c_str(), "rb");

    if (!f) {
        log::error("Could not open file: ", inputFilePath.generic_string());
        return -1;
    }

    log::debug("Allocate memory for the frames");
    picture = av_frame_alloc();

    if (!picture) {
        log::error("Could not allocate video frame");
        exit(-1);
    }

    log::debug("Reset read positions to 0");
    this->read_x = 0;
    this->read_y = 0;

    bool has_header = false;
    while (av_read_frame(format, pkt) >= 0) {
        if (pkt->stream_index == videoStream) {
            ret = avcodec_send_packet(context, pkt);

            if (ret < 0) {
                log::error("Error sending a packet to the decoder.");
                exit(-1);
            }

            while (ret >= 0) {
                ret = avcodec_receive_frame(context, picture);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                    log::info("Failed to receive frame from packet");
                    break;
                }
                if (ret >= 0) {
                    log::debug("Dealing with the frame");
                    frame_header *h = this->read_steg_header(picture);
                    if (h != nullptr) {
                        log::debug("Setting frame_header: {}", h->to_string());
                        if (this->subtitleFile->get_path().empty()) {
                            this->subtitleFile = new subtitle(h->filename, false);
                        }
                        this->subtitleFile->header = h;
                        has_header = true;
                    } else {
                        has_header = false;
                    }
                    av_frame_unref(frame);
                    ret = -1;
                }
            }
            break;

        }
        av_packet_unref(pkt);
        break;
    }
    fclose(f);

    av_parser_close(parser);
    avcodec_free_context(&context);
    av_frame_free(&picture);
    av_packet_free(&pkt);

    this->checked_header = true;
    return has_header;
}
