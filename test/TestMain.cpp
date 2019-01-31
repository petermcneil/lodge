#define CATCH_CONFIG_MAIN

#include <VideoFile.h>
#include "catch.hpp"
#include "boost/filesystem.hpp"

using namespace boost::filesystem;

path current_dir(current_path());

int framesInDirectory() {
    int count = 0;

    directory_iterator end_itr;

    for (directory_iterator itr(current_dir);
         itr != end_itr;
         ++itr) {

        string ext(itr->path().extension().c_str());
        if (ext.find(".pgm") != std::string::npos) {
            count++;
        }
    }

    return count;
}

TEST_CASE("Save frames outputs five") {
    path file_path = current_dir;
    file_path.append("samples/night/Time Lapse Video Of Night Sky.avi");

    VideoFile *video = new VideoFile(file_path, current_dir);
    int result = video->saveFrames(7);
    REQUIRE(result == 0);
    REQUIRE(framesInDirectory() == 5);
}