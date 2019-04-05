#include <spdlog/spdlog.h>
#include <boost/algorithm/string/split.hpp>
#include "frame_header.h"

using namespace lodge;
namespace log = spdlog;

const std::regex frame_header::header_regex = std::regex(R"(\|L\|(.*)\|(.*)\|L\|)");

frame_header::frame_header(long data_in_frame, string * filename, long total_frames, long frame_number) {
    this->char_data = data_in_frame;
    this->filename = *filename;
    this->total_frames = total_frames;
    this->frame_number = frame_number;
}

frame_header::frame_header(string header_string) {
    header_string = regex_replace(header_string, regex("(\\|L\\|)"), "");
    vector<string> split_header_string;

    boost::split(split_header_string, header_string, [](char c) { return c == '|'; });

    for (unsigned long iter = 0; iter < split_header_string.size(); ++iter) {
        string found = split_header_string[iter];
        if (iter == 0) {
            this->char_data = std::stol(split_header_string[iter]);
        } else if (iter == 1) {
            this->filename = split_header_string[iter];
        } else if(iter == 2) {
            this->total_frames = std::stoi(split_header_string[iter]);
        } else if(iter == 3) {
            this->frame_number = std::stoi(split_header_string[iter]);
        } else {
            break;
        }
    }
}


string frame_header::to_string() {
    ostringstream header_string;
    header_string << "|L|";
    header_string << char_data << "|";
    header_string << filename      << "|";
    header_string << total_frames  << "|";
    header_string << frame_number;
    header_string << "|L|";
    return header_string.str();
}

size_t frame_header::size() {
    return this->to_string().size() * 8;
}
