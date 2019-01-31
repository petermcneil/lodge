#ifndef LODGE_VIDEOFILE_H
#define LODGE_VIDEOFILE_H

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/avutil.h>
};

#include <string>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <spdlog/spdlog.h>

using namespace std;
using namespace boost;

class VideoFile {
    filesystem::path inputFilePath;
    filesystem::path outputFilePath;

private:
    int savePgmFrame(AVFrame *frame, AVCodecContext *context);

    int decode(AVPacket *pkt, AVCodecContext *codecContext, AVFrame *frame);

public:
    VideoFile(string videoFilePath,
              string outputFilePath);

    VideoFile(filesystem::path videoFilePath,
              filesystem::path outputFilePath);

    int saveFrames(int framesToSave);

    void delete_saved_frames();
};


#endif