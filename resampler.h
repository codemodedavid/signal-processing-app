#ifndef RESAMPLER_H
#define RESAMPLER_H

#include "signal.h"

class Resampler {
private:
    static float* reconstruct(const Signal& signal, long center, long vecinity, float time);
public:
    static Signal resample(const Signal& signal, float new_sampling_rate);
};

#endif