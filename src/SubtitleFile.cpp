#include <utility>
#include <vector>

#include "SubtitleFile.h"

SubtitleFile::SubtitleFile(filesystem::path sp) {
    this->subtitleFile = new ifstream(sp.generic_string());
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


