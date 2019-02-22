#ifndef LODGE_SUBTITLEFILE_H
#define LODGE_SUBTITLEFILE_H

#include <boost/filesystem.hpp>
#include <iostream>

using namespace std;
using namespace boost;

class SubtitleFile {
private:
    ifstream *subtitleFile;
public:
    SubtitleFile(filesystem::path subtitlePath);

    ~SubtitleFile();

    bitset<8> char_to_bin(char c);

    char bin_to_char(bitset<8> i);
};


#endif //LODGE_SUBTITLEFILE_H
