#include <VideoFile.h>
#include <SubtitleFile.h>
#include "boost/filesystem.hpp"
#include "catch.hpp"
#include <vector>

using namespace boost::filesystem;
using namespace lodge;
using namespace std;

path night_video1("/Users/mcneip01/uni/CI301/lodge/samples/night/Time Lapse Video Of Night Sky.avi");
path subtitle_file1("/Users/mcneip01/uni/CI301/lodge/samples/night/subtitle.srt");
path output_file1("/Users/mcneip01/uni/CI301/lodge/build/test");

SubtitleFile *sub = new SubtitleFile(subtitle_file1);


TEST_CASE("Character to binary conversion") {
    map<char, bitset<8>> abcMap = {
            {'a', bitset<8>{string("01100001")}}};
//            {'b',   {0, 1, 1, 0, 0, 0, 1, 0}},
//            {'c',   {0, 1, 1, 0, 0, 0, 1, 1}},
//            {'d',   {0, 1, 1, 0, 0, 1, 0, 0}},
//            {'A',   {0, 1, 0, 0, 0, 0, 0, 1}},
//            {'NUL', {0, 0, 0, 0, 0, 0, 0, 0}}
//    };
    vector<string> abc = {"01100001", "01100010", "01100011", "01100100",
                          "01100101", "01100110", "01100111", "01101000",
                          "01101001", "01101010", "01101011", "01101100",
                          "01101101", "01101110", "01101111", "01110000",
                          "01110001", "01110010", "01110011", "01110100",
                          "01110101", "01110110", "01110111", "01111000", "01111001", "01111010"};

    vector<string> bigAbc = {"01000001", "01000010", "01000011", "01000011",
                             "01000101", "01000110", "01000111", "01001000",
                             "01001001", "01001010", "01001011", "01001100",
                             "01001101", "01001110", "01001111", "01010000",
                             "01010001", "01010010", "01010011", "01010100",
                             "01010101", "01010110", "01010111", "01011000", "01011001", "01011010"};

    for (const auto &pair : abcMap) {
        bitset<8> ret = sub->char_to_bin(pair.first);
        REQUIRE(pair.second == ret);
    }

}

TEST_CASE("Binary to character conversion") {
    char ret = sub->bin_to_char(bitset<8>{string("01100001")});

    REQUIRE('a' == ret);
}

