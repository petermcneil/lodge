#include <movest_algo.h>
#include <spdlog/spdlog.h>

namespace log = spdlog;

void movest_algo::init_algo(const char *sf, bool read) {
    log::debug("Init algo");
    auto mode = lodge::RW::READ;

    if(!read) {
        mode = lodge::RW::WRITE;
    }

    log::debug("Initialising algorithm: '{}' - read: {}", sf, read);
    this->subtitle_file = new lodge::subtitle(std::string(sf), mode);
}

void movest_algo::encode(int16_t (*mvs)[2], uint16_t *mb_type, int mb_width, int mb_height, int mv_stride) {
    log::info("encode called");
    for (int mb_y = 0; mb_y < mb_height; ++mb_y) {
        for (int mb_x = 0; mb_x < mb_width; ++mb_x) {
            int xy = mb_y * mv_stride + mb_x;
            if (mb_type[xy] == 2) {
                embedIntoMv(&mvs[xy][0], &mvs[xy][1]);
            }
        }
    }
}

void movest_algo::decode(int16_t (*mvs[2])[2], uint32_t *mbtype_table, int mv_sample_log2, int mb_width, int mb_height,
                         int mv_stride, int mb_stride) {
    log::info("decode called");
    for (int mb_y = 0; mb_y < mb_height; mb_y++) {
        for (int mb_x = 0; mb_x < mb_width; mb_x++) {
            int xy = (mb_x + mb_y * mv_stride) << mv_sample_log2;
            if (mbtype_table[mb_x + mb_y * mb_stride] != 1) {
                extractFromMv(mvs[0][xy][0], mvs[0][xy][1]);
            }
        }
    }
}

void movest_algo::embedIntoMv(int16_t *mvX, int16_t *mvY) {
    processMvComponentEmbed(mvX);
    processMvComponentEmbed(mvY);
}

void movest_algo::extractFromMv(int16_t mvX, int16_t mvY) {
    processMvComponentExtract(mvX);
    processMvComponentExtract(mvY);
}


void movest_algo::processMvComponentEmbed(int16_t *mv) {
    if (stopEmbedding) return;
    bool data = this->read_next_bit();

    if(data) {
        bool success = embedIntoMvComponent(mv, bit_to_write);
        if (success) {
            log::trace("Bit successfullly written: {}", bit_to_write);
        }
    }
}

bool movest_algo::embedIntoMvComponent(int16_t *mv, int bit) {
    // Equivalent to setting the LSB of '*mv' to the one of 'bit'.
    if ((bit & 1) && !(*mv & 1)) (*mv)++;
    if (!(bit & 1) && (*mv & 1)) (*mv)--;
    return true;
}

void movest_algo::processMvComponentExtract(int16_t mv) {
    int bit = 0;
    int ex = 0;
    bool success = extractFromMvComponent(mv, &bit);
    if (success) {
        ex |= (bit & 1) << (index % 8);
        index++;
        bitsProcessed++;
        this->write_next_bit(ex);
    }
}

bool movest_algo::extractFromMvComponent(int16_t val, int *bit) {
    *bit = val;
    return true;
}


//void initAsEncoder(movest_params *params) {
//    Algorithm::initAsEncoder(params);
//    if (!(flags & MOVEST_DUMMY_PASS)) {
//        this->getDataToEmbed();
//    }
//}
//
//void initAsDecoder(movest_params *params) {
//    Algorithm::initAsDecoder(params);
//}
//
//
bool movest_algo::read_next_bit() {
    bool success = false;
    if(read_line->empty()) {
        success = this->read_next_line();

        if(success) {
            this->read_from_bs();
        } else {
            log::debug("Reached end of file");
        }
    } else {
        this->read_from_bs();
        success = true;
    }
    return success;
}

void movest_algo::read_from_bs() {
    if (read_bs_count < 8) {
        read_bs_count++;
        bit_to_write = read_bs[read_bs_count];
    } else {
        read_line->front();
        read_line->erase(read_line->begin());
    }
}
bool movest_algo::read_next_line() {
    if (subtitle_file->has_next_line()) {
        read_line = subtitle_file->next_line_bs();
        return true;
    } else {
        return false;
    }
}

void movest_algo::write_next_bit(int bit) {
    if (write_bs_count < 8) {
        read_bs.set(write_bs_count, bit);
        ++write_bs_count;
    } else {
        this->write_next_line();
    }
}


void movest_algo::write_next_line() {
    char c = lodge::subtitle::bin_to_char(write_bs);
    log::info("Character: '{}'", c);
    if (c != '\n') {
        this->subtitle_file->write_line(*write_line);
    } else {
        write_line->push_back(c);
    }
    write_bs_count = 0;
    write_bs.reset();
}

//
//void writeRecoveredData() {
//    if (index == sizeof(symb) * 8) {
//        datafile.write(symb, sizeof(symb));
//        std::fill(symb, symb + sizeof(symb), 0);
//        index = 0;
//    }
//}
//
//movest_result HideSeek::finalise() {
//    if (!encoder && index > 0 && index / 8 > 0) {
//        datafile.write(symb, index / 8);
//    }
//    return Algorithm::finalise();
//}