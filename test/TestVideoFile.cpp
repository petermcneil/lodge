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

void compare_files(const std::string &f, std::string s) {
    auto *first = new std::fstream(f, std::fstream::in);
    auto *second = new std::fstream(s, std::fstream::in);
    string first_line;
    string second_line;
    while (getline(*first, first_line)) {
        getline(*second, second_line);
        if (first_line != second_line) {
            log::info("I: {}", first_line);
            log::info("O: {}", second_line);
        }

        assert(first_line == second_line);
    }

    first->clear();
    first->seekg(0);

    second->clear();
    second->seekg(0);
}


void input_equals_output(string i_video, string o_video, string i_subtitle, string o_subtitle) {
    subtitle *i_sub = new subtitle(i_subtitle, RW::READ);
    subtitle *o_sub = new subtitle(o_subtitle, RW::WRITE);

    video *i_v = new video(i_video, o_video, i_sub);
    video *o_v = new video(o_video, o_sub);

    i_v->write_subtitle_file();
    assert(o_v->has_steg_file());
    o_v->read_subtitle_file();

    compare_files(i_subtitle, o_subtitle);
    compare_files(o_subtitle, i_subtitle);
}

void input_eq_output(string i_subtitle, string o_subtitle) {
    string input_video("extras/samples/videos/Time Lapse Video Of Night Sky.mp4");
    string output_video("output/test.mp4");

    input_equals_output(input_video, output_video, i_subtitle, o_subtitle);
}


TEST_CASE("Input subtitle file equals output subtitle file") {

    SECTION("Actual subtitle file") {
        string i_actual("extras/samples/subtitles/actual_subtitle_file.srt");
        string o_actual("output/actual_subtitle_file.srt");
        input_eq_output(i_actual, o_actual);
    }

    SECTION("Proper subtitle file") {
        string i_proper("extras/samples/subtitles/proper_test.srt");
        string o_proper("output/proper_test.srt");
        input_eq_output(i_proper, o_proper);
    }

    SECTION("GOT 25") {
        string i_got_25("extras/samples/subtitles/got_s01e25.srt");
        string o_got_25("output/got_s01e25.srt");
        input_eq_output(i_got_25, o_got_25);
    }

    SECTION("GOT S01E01") {
        input_eq_output("extras/samples/subtitles/got_s01e01.srt", "output/got_s01e01.srt");
    }

}