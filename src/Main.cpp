#include <stdio.h>
#include "VideoFile.h"
#include <string>
#include <boost/filesystem.hpp>
#include <iostream>

using namespace std;
using namespace boost;

int main(int argc, char *argv[]) {
    string currentPathS = filesystem::current_path().generic_string();
    std::size_t lodge = currentPathS.find("lodge");

    currentPathS = currentPathS.substr(0, lodge + 5);

    filesystem::path input(currentPathS + "/samples/night/Time Lapse Video Of Night Sky.avi");

    filesystem::path output(currentPathS);

    VideoFile *video = new VideoFile(input, output);
    return video->saveFrames(7);
}
