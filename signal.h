#ifndef SIGNAL_H

#define SIGNAL_H

#include "signal_data.h"
#include <cstdio>
#include <cmath>

class Signal {
    char* name;
    SignalData data;
    float** channel;

    static float decode8(const char* data);
    static float decode16(const char* data);
    static float decode32(const char* data);

    static int8_t encode8(float data);
    static int16_t encode16(float data);
    static int32_t encode32(float data);

    float* reconstruct(long center, long vecinity, float time) const;
    
public:
    Signal(int number_of_channels, long number_of_samples);
    Signal(const char* name);
    ~Signal();
    Signal(const Signal& other);
    
    Signal operator*(const Signal& other);

    Signal& resample(float new_sampling_rate);

    void printData() const;
};

#endif