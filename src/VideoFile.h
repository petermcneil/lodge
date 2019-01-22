#ifndef LODGE_VIDEOFILE_H
#define LODGE_VIDEOFILE_H

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
};

#include <string>
using namespace std;

class VideoFile {
    string filename;
private:
    void saveFrame(AVFrame *pFrame, int width, int height, int iFrame);

public:
    VideoFile(string filename);

    int saveFrames();
};


#endif