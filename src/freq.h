//
// Created by kfl02 on 31.01.20.
//

#ifndef ARDUINOSID_FREQ_H
#define ARDUINOSID_FREQ_H

#include <cmath>

class Frequency {
private:
    static const float P_CENT = 1.0005777895;
    static const float P_HALFTONE = 1.059463094;

public:
    /**
     * Increase/decrease a given frequency by a number of tempered semitones.
     *
     * @param fq the frequency
     * @param n  the number of semitones
     * @return modified frequency
     */
    inline static float const addHalftones(const float fq, const int n) {
        return fq * pow(P_HALFTONE, (float) n);
    }

    /**
     * Increase/decrease a given frequency by a scalable number of cents.
     *
     * @param fq    the frequency
     * @param n     the number of cents
     * @param scale the scale to use.
     * @return frequency in/decreased by (n * scale) cents
     */
    inline static float const addCents(const float fq, const int n, const float scale = 1.0) {
        return fq * pow(P_CENT, scale * (float) n);
    }
};

#endif //ARDUINOSID_FREQ_H
