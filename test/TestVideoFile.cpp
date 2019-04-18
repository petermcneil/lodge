#include <video.h>
#include <boost/filesystem.hpp>
#include <catch.hpp>
#include <vector>
#include <fstream>
#include <iostream>

using namespace boost::filesystem;
using namespace lodge;
using namespace std;
namespace log = spdlog;

string input_video("extras/samples/videos/Time Lapse Video Of Night Sky.mp4");
string output_video("test.mp4");
string input_subtitle("extras/samples/subtitles/got_s01e25.srt");
string output_subtitle("sub.srt");

//TEST_CASE("Written data is same as input") {
//    subtitle *i_sub = new subtitle(input_subtitle, true);
//    video *i_v = new video(input_video, output_video, i_sub);
//
//
//}

TEST_CASE("Input subtitle file equals output subtitle file") {
    subtitle *i_sub = new subtitle(input_subtitle, true);
    subtitle *o_sub = new subtitle(output_subtitle, false);

    video *i_v = new video(input_video, output_video, i_sub);
    video *o_v = new video(output_video, o_sub);

    i_v->write_subtitle_file();
    assert(o_v->has_steg_file());
    o_v->read_subtitle_file();
    auto *r_i = new std::fstream(input_subtitle, std::fstream::in);
    auto *r_o = new std::fstream(output_subtitle, std::fstream::in);

    string input_line;
    string output_line;
    while (getline(*r_i, input_line)) {
        getline(*r_o, output_line);
        if(input_line != output_line) {
            log::info("I: {}", input_line);
            log::info("O: {}", output_line);
        }

        assert(input_line == output_line);
    }
}