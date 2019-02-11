#include "Encoder.h"

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

unsigned long Encoder::flatten_bit(unsigned long &input) {
    return input &= ~(1UL << 0UL);
}

byte replace_bit(byte i, byte r) {
    unsigned long input = 100001UL;

    Encoder::flatten_bit(input);

    return i;
}

