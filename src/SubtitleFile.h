#ifndef LODGE_SUBTITLEFILE_H
#define LODGE_SUBTITLEFILE_H

#include <boost/filesystem.hpp>
#include <fstream>
#include <iostream>

namespace lodge {
    using namespace std;
    using namespace boost;

    class SubtitleFile {
    private:
        filesystem::path filePath;
        fstream *subtitleFile;
        bool readOnly;
        bitset<8> endline = bitset<8>{string("00001010")};
    public:
        explicit SubtitleFile(filesystem::path subtitlePath, bool readOnly);

        ~SubtitleFile();

        static bitset<8> char_to_bin(char c);

        static char bin_to_char(bitset<8> i);

        vector<bitset<8>> *read_next_line();

        int write_line(vector<char> lineCharacters);

        bool has_next_line();
    };
}

#endif //LODGE_SUBTITLEFILE_H
