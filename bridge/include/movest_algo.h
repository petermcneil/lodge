#ifndef LODGE_MOVEST_ALGO_H
#define LODGE_MOVEST_ALGO_H

#include <memory>
#include <vector>
#include <stdint.h>
#include <ffmpeg_connector.h>
#include <subtitle.h>

class movest_algo {
public:
    virtual void encode(int16_t (*mvs)[2], uint16_t *mb_type, int mb_width, int mb_height, int mv_stride);

    virtual void decode(int16_t (*mvs[2])[2], uint32_t *mbtype_table, int mv_sample_log2, int mb_width, int mb_height,
                        int mv_stride, int mb_stride);

    virtual void embedIntoMv(int16_t *mvX, int16_t *mvY);

    virtual void extractFromMv(int16_t mvX, int16_t mvY);

    virtual bool embedIntoMvComponent(int16_t *mv, int bit);

    virtual bool extractFromMvComponent(int16_t mv, int *bit);

    virtual void processMvComponentEmbed(int16_t *mv);

    virtual void processMvComponentExtract(int16_t mv);

    void init_algo(const char *sf, bool read);

    virtual ~movest_algo() = default;
protected:
    bool stopEmbedding = false;
    uint indexLimit = 0;
    uint index = 0;
    unsigned long bitsProcessed = 0;
    int flags = 0;
    bool encoder = false;

    bitset<8> write_bs;
    vector<char> *write_line = new vector<char>;
    size_t write_bs_count = 0;

    int bit_to_write = 0;
    bitset<8> read_bs;
    vector<bitset<8>> *read_line = new vector<bitset<8>>;
    size_t read_bs_count = 0;

    lodge::subtitle * subtitle_file = nullptr;

    bool read_next_bit();

    bool read_next_line();

    void read_from_bs();

    void write_next_bit(int bit);

    void write_next_line();

};

#endif //LODGE_MOVEST_ALGO_H
