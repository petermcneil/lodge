#ifndef LODGE_SUBTITLE_H
#define LODGE_SUBTITLE_H

#include <boost/filesystem.hpp>
#include <regex>
#include <fstream>
#include <iostream>
#include "frame_header.h"

namespace lodge {


    enum class RW {
        READ,
        WRITE
    };

    class subtitle {
    private:
        boost::filesystem::path file_path;
        std::string * filename;
        std::fstream *subtitle_file;
        RW rw;
        const static std::bitset<8> new_line;

        std::string current_line;
        int read_next_line();
        //Flag to indicate whether the current line has been written
        bool written_current_line = false;

    public:
        //Number of characters
        long size;

        explicit subtitle(std::string subtitlePath, RW rw);

        explicit subtitle(const boost::filesystem::path& subtitlePath, RW rw);

        ~subtitle();

        static std::bitset<8> char_to_bin(char c);

        static char bin_to_char(std::bitset<8> i);

        std::vector<std::bitset<8>> *next_line_bs();

        int write_line(const std::vector<char>& lineCharacters);

        bool has_next_line();

        boost::filesystem::path get_path();

        void set_path(std::string path);

        std::string * get_filename();

        size_t next_line_length();
    };
}

#endif //LODGE_SUBTITLE_H
