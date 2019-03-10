#include <utility>

#include <utility>
#include <vector>
#include <spdlog/spdlog.h>
#include <sstream>
#include <string>
#include "SubtitleFile.h"
#include <boost/algorithm/string.hpp>

using namespace lodge;
namespace log = spdlog;

const std::regex Header::header_regex = std::regex(R"(\|LODGE\|(.*)\|(.*)\|LODGE\|)");
const std::regex Header::start_end_regex = std::regex(R"(\|LODGE\|(.*)\|LODGE\|)");
const bitset<8> SubtitleFile::new_line = bitset<8>{string("00001010")};

SubtitleFile::SubtitleFile(string subtitlePath, bool readOnly) : SubtitleFile(filesystem::path(subtitlePath),
                                                                              readOnly) {}

SubtitleFile::SubtitleFile(filesystem::path sp, bool readOnly) {
    this->read_only = readOnly;
    if (this->read_only) {
        log::debug("Read only subtitle file");
        this->file_path = canonical(sp);
        this->subtitle_file = new fstream(this->file_path.generic_string(), fstream::ate | fstream::in);

        subtitle_file->seekg(0, std::ios_base::end);
        streampos end_pos = subtitle_file->tellg();
        subtitle_file->seekg(0);
        this->size = end_pos;
        this->header = new Header(this->size, file_path.extension().generic_string());
    } else {
        log::debug("Write only subtitle file");
        this->file_path = weakly_canonical(sp);
        this->subtitle_file = new fstream(this->file_path.generic_string(),
                                          fstream::ate | fstream::out | fstream::trunc);
        this->header = nullptr;
    }
}

SubtitleFile::~SubtitleFile() {
    this->subtitle_file->close();
}

bitset<8> SubtitleFile::char_to_bin(char c) {
    return bitset<8>(static_cast<unsigned long long int>(c));
}

char SubtitleFile::bin_to_char(bitset<8> i) {
    return static_cast<char>(i.to_ulong());
}

vector<bitset<8>> *SubtitleFile::read_next_line() {
    if (!this->read_only) {
        log::error("The file ({}) is to be written to, not read from.", this->file_path.generic_string());
        throw "File is write only.";
    } else if (!subtitle_file->is_open()) {
        log::error("The file ({}) is not open.", this->file_path.generic_string());
        throw "File not open or doesn't exist.";
    } else {
        auto *data = new vector<bitset<8>>;
        string line;
        while (getline(*subtitle_file, line)) {
            for (char &ch: line) {
                auto converted = this->char_to_bin(ch);
                data->push_back(converted);
            }
            break;
        }
        data->push_back(new_line);
        return data;
    }
}

int SubtitleFile::write_line(vector<char> lineCharacters) {
    if (this->read_only) {
        log::error("The file ({}) is to be read from, not written to.", this->file_path.generic_string());
        return 23;
    } else if (!subtitle_file->is_open()) {
        log::error("The file ({}) is not open.", this->file_path.generic_string());
        return 12;
    } else {
        string line;
        for (auto &character: lineCharacters) {
            if (character != '\n') {
                line += character;
            }
        }
        log::debug("Adding this to the file: {}", line.c_str());
        *subtitle_file << line << std::endl;
        return 0;
    }
}

bool SubtitleFile::has_next_line() {
    return subtitle_file->peek() != EOF;
}

Header::Header(long size, string extension) {
    this->size = size;
    this->extension = std::move(extension);
}

Header::Header(string header_string) {
    header_string = regex_replace(header_string, regex("(\\|LODGE\\|)"), "");
    vector<string> split_header_string;

    log::debug("Header string: {}", header_string);
    boost::split(split_header_string, header_string, [](char c) { return c == '|'; });

    for (unsigned long iter = 0; iter < split_header_string.size(); ++iter) {
        string found = split_header_string[iter];
        if (iter == 0) {
            this->size = std::stol(split_header_string[iter]);
        } else if (iter == 1) {
            this->extension = split_header_string[iter];
        }
    }
}


string Header::to_string() {
    ostringstream header_string;
    header_string << "|LODGE|";
    header_string << size << "|";
    header_string << extension;
    header_string << "|LODGE|";
    return header_string.str();
}