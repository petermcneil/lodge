#ifndef LODGE_SUBTITLEFILE_H
#define LODGE_SUBTITLEFILE_H

#include <boost/filesystem.hpp>
#include <regex>
#include <fstream>
#include <iostream>

namespace lodge {
    using namespace std;
    using namespace boost;

    class frame_header {
    public:
        long size;
        string extension;

        frame_header(long size, string extension);

        explicit frame_header(string header_string);

        string to_string();

        const static std::regex start_end_regex;
        const static std::regex header_regex;
    };

    class subtitle {
    private:
        filesystem::path file_path;
        fstream *subtitle_file;
        bool read_only;
        const static bitset<8> new_line;
    public:
        frame_header *header;
        long size;

        explicit subtitle(string subtitlePath, bool readOnly);

        explicit subtitle(filesystem::path subtitlePath, bool readOnly);

        ~subtitle();

        static bitset<8> char_to_bin(char c);

        static char bin_to_char(bitset<8> i);

        vector<bitset<8>> *read_next_line();

        int write_line(vector<char> lineCharacters);

        bool has_next_line();
    };
}

#endif //LODGE_SUBTITLEFILE_H
