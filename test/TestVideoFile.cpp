#include <VideoFile.h>
#include "boost/filesystem.hpp"
#include "catch.hpp"
#include <vector>

using namespace boost::filesystem;
using namespace lodge;

path night_video("samples/night/Time Lapse Video Of Night Sky.avi");
path subtitle_file("samples/night/subtitle.srt");
path output_file("build/test");

int framesInDirectory() {
    int count = 0;

    directory_iterator end_itr;

    for (directory_iterator itr(output_file);
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
    VideoFile *video = new VideoFile(night_video, output_file, nullptr);
    int result = video->saveFrames(7);
    REQUIRE(result == 0);

    REQUIRE (framesInDirectory() == 5);

    video->delete_saved_frames();
}