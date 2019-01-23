#include <stdio.h>
#include "VideoFile.h"
#include <string>
#include <boost/filesystem.hpp>

using namespace std;
using namespace boost;

int main(int argc, char *argv[]) {
    filesystem::path dir = filesystem::current_path();

    dir.append("samples/night/Time Lapse Video Of Night Sky.avi");
    VideoFile *video = new VideoFile(dir);
    return video->saveFrames();
}
