#include <utility>
#include <vector>
#include <spdlog/spdlog.h>
#include <string>
#include "subtitle.h"
#include <boost/algorithm/string.hpp>

using namespace lodge;
using namespace std;
using namespace boost;
namespace log = spdlog;

const bitset<8> subtitle::new_line = bitset<8>{string("00001010")};

subtitle::subtitle(string subtitlePath, RW rw) : subtitle(filesystem::weakly_canonical(subtitlePath),
                                                          rw) {}

subtitle::subtitle(const filesystem::path &sp, RW rw) {
    this->rw = rw;
    if (this->rw == RW::READ) {
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
            if(!filesystem::exists(sp.parent_path())) {
                log::debug("Directory ({}) doesn't exist - creating it", sp.parent_path().c_str());
                filesystem::create_directory(sp.parent_path());
            }
            this->file_path = weakly_canonical(sp);
            this->filename = new string(this->file_path.filename().generic_string());
            this->subtitle_file = new fstream(this->file_path.c_str(),
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

/**
 *
 * Error table
 * | int| reason           |
 * | -1 | Wrong mode       |
 * | -2 | File is not open |
 *
 * @return 0 for success, anything else error (see table)
 */
int subtitle::read_next_line() {
    if (this->rw == RW::WRITE) {
        log::error("The file ({}) is to be written to, not read from.", this->file_path.generic_string());
        return -1;
    } else if (!subtitle_file->is_open()) {
        log::error("The file ({}) is not open.", this->file_path.generic_string());
        return -2;
    } else {
        current_line.erase();
        getline(*subtitle_file, current_line);
        this->written_current_line = true;
        return 0;
    }
}

vector<bitset<8>> *subtitle::next_line_bs() {
    if (this->written_current_line) {
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

/**
 *
 * Error table
 * | int| reason           |
 * | -1 | Wrong mode       |
 * | -2 | File is not open |
 *
 * @param lineCharacters
 * @return 0 for success, anything else error (see table)
 */
int subtitle::write_line(const vector<char> &lineCharacters) {
    if (this->rw == RW::READ) {
        log::error("The file ({}) is to be read from, not written to.", this->file_path.generic_string());
        return -1;
    } else if (!subtitle_file->is_open()) {
        log::error("The file ({}) is not open.", this->file_path.generic_string());
        return -2;
    } else {
        string line;
        for (auto &character: lineCharacters) {
            if (character != '\n') {
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
    this->subtitle_file = new fstream(this->file_path.generic_string(),
                                      fstream::ate | fstream::out | fstream::trunc);
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
