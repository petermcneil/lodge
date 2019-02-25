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
        ifstream *subtitleFile;
        bool readOnly;
    public:
        explicit SubtitleFile(filesystem::path subtitlePath);

        ~SubtitleFile();

        static bitset<8> char_to_bin(char c);

        static char bin_to_char(bitset<8> i);

        vector<bitset<8>> *read_next_line();
    };
}

#endif //LODGE_SUBTITLEFILE_H
