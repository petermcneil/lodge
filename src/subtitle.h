#ifndef LODGE_SUBTITLEFILE_H
#define LODGE_SUBTITLEFILE_H

#include <boost/filesystem.hpp>
#include <regex>
#include <fstream>
#include <iostream>
#include "frame_header.h"

namespace lodge {
    using namespace std;
    using namespace boost;

    enum class RW {
        READ,
        WRITE
    };

    class subtitle {
    private:
        filesystem::path file_path;
        string * filename;
        fstream *subtitle_file;
        RW rw;
        const static bitset<8> new_line;

        string current_line;
        int read_next_line();
        //Flag to indicate whether the current line has been written
        bool written_current_line = false;

    public:
        //Number of characters
        long size;

        explicit subtitle(string subtitlePath, RW rw);

        explicit subtitle(filesystem::path subtitlePath, RW rw);

        ~subtitle();

        static bitset<8> char_to_bin(char c);

        static char bin_to_char(bitset<8> i);

        vector<bitset<8>> *next_line_bs();

        int write_line(vector<char> lineCharacters);

        bool has_next_line();

        filesystem::path get_path();

        void set_path(string path);

        string * get_filename();

        size_t next_line_length();
    };
}

#endif //LODGE_SUBTITLEFILE_H
