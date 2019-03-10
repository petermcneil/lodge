#include <video.h>
#include "boost/filesystem.hpp"
#include "catch.hpp"
#include <vector>
#include <fstream>
#include <iostream>

using namespace boost::filesystem;
using namespace lodge;
using namespace std;

path input_video("extras/samples/videos/Time Lapse Video Of Night Sky.mp4");
path output_video("test.mp4");
path input_subtitle("extras/samples/subtitles/actual_subtitle_file.srt");
path output_subtitle("test.srt");

TEST_CASE("Input subtitle file equals output subtitle file") {
    subtitle *i_sub = new subtitle(input_subtitle, true);
    subtitle *o_sub = new subtitle(output_subtitle, false);

    video *i_v = new video(input_video, output_video, i_sub);
    video *o_v = new video(input_video, output_video, o_sub);

    i_v->write_subtitle_file();
    o_v->read_subtitle_file();
    auto *r_i = new std::fstream(input_subtitle.generic_string(), std::fstream::ate | std::fstream::in);
    auto *r_o = new std::fstream(output_subtitle.generic_string(), std::fstream::ate | std::fstream::in);

    string input_line;
    string output_line;
    while (getline(*r_i, input_line)) {
        getline(*r_o, output_line);
        assert(input_line == output_line);
    }
}