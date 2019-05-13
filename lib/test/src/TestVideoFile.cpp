#include <utility>

#include "video.h"
#include <boost/filesystem.hpp>
#include <catch.hpp>
#include <vector>
#include <fstream>
#include <iostream>

using namespace boost::filesystem;
using namespace lodge;
using namespace std;
namespace log = spdlog;

void compare_files(const std::string &f, const std::string& s) {
    auto *first = new std::fstream(f, std::fstream::in);
    auto *second = new std::fstream(s, std::fstream::in);
    string first_line;
    string second_line;
    while (getline(*first, first_line)) {
        getline(*second, second_line);
        if (first_line != second_line) {
            log::error("I: {}", first_line);
            log::error("O: {}", second_line);
        }

        REQUIRE(first_line == second_line);
    }

    first->clear();
    first->seekg(0);

    second->clear();
    second->seekg(0);
}


void input_equals_output(string i_video, const string& o_video, string i_subtitle, string o_subtitle) {
    subtitle *i_sub = new subtitle(i_subtitle, RW::READ);
    subtitle *o_sub = new subtitle(o_subtitle, RW::WRITE);

    video *i_v = new video(std::move(i_video), o_video, i_sub);
    video *o_v = new video(o_video, o_sub);

    i_v->write_subtitle_file();
    REQUIRE(o_v->has_steg_file());
    o_v->read_subtitle_file();

    compare_files(i_subtitle, o_subtitle);
    compare_files(o_subtitle, i_subtitle);
}

void input_eq_output(string i_subtitle, string o_subtitle) {
    string input_video("resources/videos/Time Lapse Video Of Night Sky.mp4");
    string output_video("output/test.mp4");

    input_equals_output(input_video, output_video, std::move(i_subtitle), o_subtitle);
}


TEST_CASE("Input subtitle file equals output subtitle file") {

    SECTION("Actual subtitle file") {
        input_eq_output("resources/subtitles/actual_subtitle_file.srt", "output/actual_subtitle_file.srt");
    }

    SECTION("Proper subtitle file") {
        input_eq_output("resources/subtitles/proper_test.srt", "output/proper_test.srt");
    }

    SECTION("GOT 25") {
        input_eq_output("resources/subtitles/got_s01e25.srt", "output/got_s01e25.srt");
    }

    SECTION("GOT S01E01") {
        input_eq_output("resources/subtitles/got_s01e01.srt", "output/got_s01e01.srt");
    }

    SECTION("Ebutt") {
        input_eq_output("resources/subtitles/ebutt.xml", "output/ebutt.xml");
    }

    SECTION("Timed text") {
        input_eq_output("resources/subtitles/timedtext.xml", "output/timedtext.xml");
    }

}