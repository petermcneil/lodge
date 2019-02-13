#include "Encoder.h"
#include <spdlog/spdlog.h>

/**
 * Inserts data into the frame using LSB
 *
 */
int Encoder::insert_into_frame(AVFrame *frame, string s) {
    return 0;
}

int Encoder::extract_from_frame(AVFrame *frame) {
    return 0;
}

void Encoder::flatten_bit(unsigned long &input) {
    input &= ~(1UL << 0UL);
}

void Encoder::write_lsb(unsigned long &input, unsigned long replacement) {
    Encoder::flatten_bit(input);
    input |= (replacement << 0);
}

unsigned long Encoder::read_lsb(unsigned long &input) {
    return (input >> 0) & 1UL;
}

