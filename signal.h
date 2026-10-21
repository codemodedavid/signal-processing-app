#ifndef SIGNAL_H

#define SIGNAL_H

#include "signal_data.h"
#include <cstdio>


class Signal {
    char* name;
    SignalData data;
    float** channel;

    static float decode8(const char* data);
    static float decode16(const char* data);
    static float decode32(const char* data);
public:
    Signal(const char* name);
    ~Signal();
    Signal(const Signal& other);

    void printData();
};

#endif