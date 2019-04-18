#include <utility>
#include <vector>
#include <spdlog/spdlog.h>
#include <string>
#include "subtitle.h"
#include <boost/algorithm/string.hpp>

using namespace lodge;
namespace log = spdlog;

const bitset<8> subtitle::new_line = bitset<8>{string("00001010")};

subtitle::subtitle(string subtitlePath, bool readOnly) : subtitle(filesystem::path(subtitlePath),
                                                                  readOnly) {}

subtitle::subtitle(filesystem::path sp, bool readOnly) {
    this->read_only = readOnly;
    if (this->read_only) {
        log::debug("Read only subtitle file");
        this->file_path = canonical(sp);
        this->filename = new string(this->file_path.filename().generic_string());
        this->subtitle_file = new fstream(this->file_path.generic_string(), fstream::ate | fstream::in);

        subtitle_file->seekg(0, std::ios_base::end);
        streampos end_pos = subtitle_file->tellg();
        subtitle_file->seekg(0);
        this->size = end_pos;
    } else {
        log::debug("Write only subtitle file");
        if (sp.empty()) {
            this->file_path = sp;
        } else {
            this->file_path = weakly_canonical(sp);
            this->filename = new string(this->file_path.filename().generic_string());
            this->subtitle_file = new fstream(this->file_path.generic_string(),
                                              fstream::ate | fstream::out | fstream::trunc);
        }

    }
}

subtitle::~subtitle() {
    this->subtitle_file->close();
}

bitset<8> subtitle::char_to_bin(char c) {
    return bitset<8>(static_cast<unsigned long long int>(c));
}

char subtitle::bin_to_char(bitset<8> i) {
    return static_cast<char>(i.to_ulong());
}

int subtitle::read_next_line() {
    if (!this->read_only) {
        log::error("The file ({}) is to be written to, not read from.", this->file_path.generic_string());
        throw "File is write only.";
    } else if (!subtitle_file->is_open()) {
        log::error("The file ({}) is not open.", this->file_path.generic_string());
        throw "File not open or doesn't exist.";
    } else {
        current_line.erase();
        getline(*subtitle_file, current_line);
        this->written_current_line = true;
        return 0;
    }
}
vector<bitset<8>> *subtitle::next_line_bs() {
    if(this->written_current_line){
        this->written_current_line = false;
    } else {
        this->read_next_line();
    }
    auto *data = new vector<bitset<8>>;
    for (char &ch: current_line) {
        auto converted = this->char_to_bin(ch);
        data->push_back(converted);
    }
    data->push_back(new_line);
    return data;
}

int subtitle::write_line(vector<char> lineCharacters) {
    if (this->read_only) {
        log::error("The file ({}) is to be read from, not written to.", this->file_path.generic_string());
        return 23;
    } else if (!subtitle_file->is_open()) {
        log::error("The file ({}) is not open.", this->file_path.generic_string());
        return 12;
    } else {
        string line;
//        log::info("Printing each character written");
        for (auto &character: lineCharacters) {
            if (character != '\n') {
//                log::info("'{}'", character);
                line += character;
            }
        }
        log::debug("Adding this to file: {}", line);
        *subtitle_file << line << std::endl;
        return 0;
    }
}

bool subtitle::has_next_line() {
    return subtitle_file->peek() != EOF;
}

filesystem::path subtitle::get_path() {
    return this->file_path;
}

void subtitle::set_path(string path) {
    this->file_path = weakly_canonical(filesystem::path(path));
}

string *subtitle::get_filename() {
    return this->filename;
}

size_t subtitle::next_line_length() {
    if (this->has_next_line()) {
        this->read_next_line();

        return current_line.length();
    }
    return static_cast<size_t>(-1);
}
