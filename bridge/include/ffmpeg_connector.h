#ifndef LODGE_FFMPEG_CONNECTOR_H
#define LODGE_FFMPEG_CONNECTOR_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
* Method called by FFmpeg to feed in motion vector data during encoding.
* (parameters match those available as variables at the point
* of injection).
*/
void lodge_encode(int16_t (*mvs)[2], uint16_t *mb_type, int mb_width, int mb_height, int mv_stride);

/**
 * Method called by FFmpeg to feed in motion vector data during decoding.
 * (parameters match those available as variables at the point
 * of injection).
 */
void lodge_decode(int16_t (*mvs[2])[2], uint32_t *mbtype_table, int mv_sample_log2, int mb_width, int mb_height,
                  int mv_stride, int mb_stride);


void lodge_init_encoder(const char *sf);
void lodge_init_decoder(const char *sf);

#ifdef __cplusplus
}
#endif
#endif //LODGE_FFMPEG_CONNECTOR_H
