#include <video.h>
#include <subtitle.h>
#include "boost/filesystem.hpp"
#include "catch.hpp"
#include <vector>
#include <iostream>
#include <fstream>

using namespace boost::filesystem;
using namespace lodge;
using namespace std;

map<char, bitset<8>> abcMap = {
        {'a',  bitset<8>{string("01100001")}},
        {'b',  bitset<8>{string("01100010")}},
        {'A',  bitset<8>{string("01000001")}},
        {'\0', bitset<8>{string("00000000")}}
};

map<string, bitset<8>> unicode = {
        {"😀", bitset<8>{string("0000000")}}
};

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

TEST_CASE("Character to binary conversion") {
    SECTION("ASCII") {
        for (const auto &pair : abcMap) {
            bitset<8> ret = lodge::subtitle::char_to_bin(pair.first);
            REQUIRE(pair.second == ret);
        }
    }
//TODO Unicode (UTF8) support
//    SECTION("UTF8") {
//        for (const auto &pair : unicode) {
//            bitset<8> ret = lodge::subtitle::char_to_bin(pair.first);
//            REQUIRE(pair.second == ret);
//        }
//    }
}

TEST_CASE("Binary to character conversion") {
    SECTION("ASCII") {
        for (const auto &pair : abcMap) {
            char ret = lodge::subtitle::bin_to_char(pair.second);
            REQUIRE(pair.first == ret);
        }
    }
}

TEST_CASE("Subtitle file can read lines out") {
    string expected1 = "abcdefghijklmnopqrstuvxyz\n";
    string expected2 = "ABCDEFGHIJKLMNOPQRSTUVXYZ\n";
    string actual1;
    string actual2;
    string read_file("resources/subtitles/test_file.srt");
    subtitle *read_sub = new subtitle(read_file, RW::READ);

    REQUIRE(read_sub->next_line_length() == 25);
    auto line1 = *(read_sub->next_line_bs());
    REQUIRE(read_sub->next_line_length() == 25);
    auto line2 = *(read_sub->next_line_bs());
    REQUIRE(read_sub->next_line_length() == -1);

    for (auto ch : line1) {
        auto bits = lodge::subtitle::bin_to_char(ch);
        actual1 += bits;
    }

    for (auto ch : line2) {
        auto bits = lodge::subtitle::bin_to_char(ch);
        actual2 += bits;
    }

    REQUIRE(!read_sub->has_next_line());
    REQUIRE(expected1 == actual1);
    REQUIRE(expected2 == actual2);
}

TEST_CASE("Subtitle file can write lines out") {
    vector<char> input = {'a', 'b', 'c', 'd', 'e', '\n'};
    vector<char> input2 = {'A', 'B', 'C', 'D', 'E', '\n'};
    string expected1 = "abcde";
    string expected2 = "ABCDE";
    string actual1;
    string actual2;

    string write_file("output/gen_subs.srt");
    subtitle *write_sub = new subtitle(write_file, RW::WRITE);

    int ret = write_sub->write_line(input);
    REQUIRE(ret == 0);
    ret = write_sub->write_line(input2);
    REQUIRE(ret == 0);

    std::ifstream output(write_file);

    while(getline(output, actual1)) {
        REQUIRE(expected1 == actual1);
        break;
    }

    while(getline(output, actual2)) {
        REQUIRE(expected2 == actual2);
        break;
    }
}
