#ifndef LODGE_SUBTITLEFILE_H
#define LODGE_SUBTITLEFILE_H

#include <boost/filesystem.hpp>
#include <regex>
#include <fstream>
#include <iostream>

namespace lodge {
    using namespace std;
    using namespace boost;

    /**
     * This header is placed at the top of each frame that is written to.
     * It details the information about the file saved inside the video.
     */
    class frame_header {
    private:
        long totalFrames;
        long frameNumber;
        long dataInFrame;

    public:
        long file_size;
        string filename;

        frame_header(long size, string filename);

        explicit frame_header(string header_string);

        string to_string();

        size_t size();

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

        filesystem::path get_path();

        void set_path(string path);
    };
}

#endif //LODGE_SUBTITLEFILE_H
