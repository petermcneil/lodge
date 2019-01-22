#include <stdio.h>
#include "VideoFile.h"
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    VideoFile *video = new VideoFile("/Users/mcneip01/uni/CI301/lodge/samples/night/Time Lapse Video Of Night Sky.avi");
    return video->saveFrames();
}
