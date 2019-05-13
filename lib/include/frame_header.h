#ifndef LODGE_FRAME_HEADER_H
#define LODGE_FRAME_HEADER_H

#include <regex>
#include <string>

namespace lodge {
    /**
     * This header is placed at the top of each frame that is written to.
     * It details the information about the file saved inside the video.
     */
    class frame_header {
    public:
        std::string filename;
        long char_data;
        long total_frames;
        long frame_number;

        frame_header(long data_in_frame, std::string * filename, long total_frames, long frame_number);

        explicit frame_header(std::string header_string);

        std::string to_string();

        size_t size();

        const static std::regex header_regex;
    };

}


#endif //LODGE_FRAME_HEADER_H
