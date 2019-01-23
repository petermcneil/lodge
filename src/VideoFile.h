#ifndef LODGE_VIDEOFILE_H
#define LODGE_VIDEOFILE_H

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
};

#include <string>
#include <boost/filesystem.hpp>

using namespace std;
using namespace boost;

class VideoFile {
    filesystem::path filePath;
private:
    void saveFrame(AVFrame *pFrame, int width, int height, int iFrame);

public:
    explicit VideoFile(string videoFilePath);

    explicit VideoFile(filesystem::path videoFilePath);

    int saveFrames();
};


#endif