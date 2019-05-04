#include <ffmpeg_connector.h>
#include <iostream>
#include <movest_algo.h>

static movest_algo *algorithm;

void lodge_encode(int16_t (*mvs)[2], uint16_t *mb_type, int mb_width, int mb_height, int mv_stride) {
    if(algorithm == nullptr) {
        std::cerr << "lodge_encode error: initialise algorithm first (_init_encoder)."
                  << std::endl;
        return;
    }
    algorithm->encode(mvs, mb_type, mb_width, mb_height, mv_stride);
}

void lodge_decode(int16_t (*mvs[2])[2], uint32_t *mbtype_table, int mv_sample_log2, int mb_width, int mb_height,
                  int mv_stride, int mb_stride) {
    if(algorithm == nullptr) {
        std::cerr << "lodge_decode error: initialise algorithm first (movest_init_decoder)."
                  << std::endl;
        return;
    }
    algorithm->decode(mvs, mbtype_table, mv_sample_log2, mb_width, mb_height, mv_stride, mb_stride);
}

void lodge_init_encoder(const char *sf) {
    std::cout << "Entering lodge bridge" << std::endl;
    algorithm = new movest_algo();
    algorithm->init_algo(sf, true);
    std::cout << "Initalised algo" << std::endl;
}
void lodge_init_decoder(const char *sf) {
    std::cout << "Entering lodge bridge" << std::endl;
    algorithm = new movest_algo();
    algorithm->init_algo(sf, false);
    std::cout << "Initalised algo" << std::endl;
}
