/**
 * how to use
 *
 * number_of_samples can only be power of two (1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096)
 * for arduino nano max is 256
 * set FFT_SAMPLE_ARRAY_MAX_SIZE to max size you will use as number_of_samples and sample_array
 *
 * notes
 *
 * accuracy in fft_private_sine() can be zero to gain more speed
 * leave 211 bytes for local variables on arduino nano when using Serial with 256 sample array size
 *
 * todo
 *
 * find faster pow() function
 **/

#ifndef FFT_H
#define FFT_H

#include "my_library.h"

#define FFT_SAMPLE_ARRAY_MAX_SIZE (256)

extern int16_t real[FFT_SAMPLE_ARRAY_MAX_SIZE];
extern int16_t imaginary[FFT_SAMPLE_ARRAY_MAX_SIZE];
extern f32_t frequency_with_highest_amplitude;

extern void fft_calculate_frequency_with_highest_amplitude(int16_t *sample_array, uint16_t sample_amount, f32_t sample_rate, bool_t is_calculate_frequency_with_highest_amplitude);

#endif /* FFT_H */
