#include <utility>
#include <vector>
#include <spdlog/spdlog.h>
#include "SubtitleFile.h"

using namespace lodge;
using namespace spdlog;

SubtitleFile::SubtitleFile(filesystem::path sp, bool readOnly) {
    this->filePath = sp.remove_trailing_separator();
    if (!readOnly) {
        this->subtitleFile = new fstream(this->filePath.generic_string(), fstream::out | fstream::trunc);
    } else {
        this->subtitleFile = new fstream(this->filePath.generic_string(), fstream::in);
    }
    this->readOnly = readOnly;
}

SubtitleFile::~SubtitleFile() {
    this->subtitleFile->close();
}

bitset<8> SubtitleFile::char_to_bin(char c) {
    return bitset<8>(static_cast<unsigned long long int>(c));
}

char SubtitleFile::bin_to_char(bitset<8> i) {
    return static_cast<char>(i.to_ulong());
}

vector<bitset<8>> *SubtitleFile::read_next_line() {
    if (!this->readOnly) {
        error("The file ({}) is to be written to, not read from.", this->filePath.generic_string());
        throw "File is write only.";
    } else if (!subtitleFile->is_open()) {
        error("The file ({}) is not open.", this->filePath.generic_string());
        throw "File not open or doesn't exist.";
    } else {
        auto *data = new vector<bitset<8>>;
        string line;
        while (getline(*subtitleFile, line)) {
            for (char &ch: line) {
                auto converted = this->char_to_bin(ch);
                data->push_back(converted);
            }
            break;
        }
        data->push_back(endline);
        return data;
    }
}

int SubtitleFile::write_line(vector<char> lineCharacters) {
    if (this->readOnly) {
        error("The file ({}) is to be read from, not written to.", this->filePath.generic_string());
        return 23;
    } else if (!subtitleFile->is_open()) {
        error("The file ({}) is not open.", this->filePath.generic_string());
        return 12;
    } else {
        string line;
        for (auto &character: lineCharacters) {
            if (character != '\n') {
                line += character;
            }
        }
        debug("Adding this to the file: {}", line.c_str());
        *subtitleFile << line << std::endl;
        return 0;
    }
}