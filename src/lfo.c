//
// Created by kfl02 on 31.01.20.
//

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

const float P_CENT = 1.0005777895;
const float P_HALFTONE = 1.059463094;

float sine(float phase) {
    return sinf(2 * M_PI * phase);
}

float saw_up(float phase) {
    return 2.0 * phase - 1.0;
}

float saw_down(float phase) {
    return -2.0 * phase + 1.0;
}

float triangle(float phase) {
    return phase < 0.25 ? phase * 4.0 :
           phase < 0.5 ? 2.0 - phase * 4.0 :
           phase < 0.75 ? -phase * 4.0 + 2.0:
           phase * 4.0 - 4.0;
}

float rectangle(float phase, float width) {
    return phase < width ? 1.0 : -1.0;
}

float phasor(float hz, float shift, long ms) {
    return fmod((float) ms + 1000.0 * shift, 1000.0 / hz) / (1000.0 / hz);
}

float arpeggiator(float hz, uint8_t n, int8_t *offsets, float phase) {
    uint8_t index = (uint8_t) (phase * (float) n);

    return hz * pow(P_HALFTONE, offsets[index]);
}

float exponential(float hz, int max, float phase) {
    return hz * pow(P_CENT, (float) max * phase);
}

float linear(float hz, int max, float phase) {
    return hz + max * phase;
}

int main(int argc, char **argv) {
    float hz = 1.0;
    float shift = 0.0;
    float freq = 440.0;
    float max = 2400;

    float hz2 = 10.0;
    float shift2 = 0.2;
    float max2 = 1200;

    int8_t arr[] = { 0, 3, 7, 12 };
    uint8_t arr_size = sizeof(arr) / sizeof(int8_t);

    for(long i = 0; i < 1000; i++) {
        float res;

        res = arpeggiator(freq, arr_size, arr, phasor(hz, shift, i));
        res = exponential(res, 100, sine(phasor(hz2, shift, i)));

        printf("%ld %f\n", i, res);
    }
}
