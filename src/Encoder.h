#ifndef LODGE_LEASTSIGBIT_H
#define LODGE_LEASTSIGBIT_H
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/avutil.h>
};

#include <string>
#include <stdlib.h>

using namespace std;
namespace Encoder {

    unsigned long flatten_bit(unsigned long &input);

    int insert_into_frame(AVFrame *frame, string s);

    int extract_from_frame(AVFrame *frame);
};


#endif //LODGE_LEASTSIGBIT_H
