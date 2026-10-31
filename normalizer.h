#ifndef NORMALIZER_H
#define NORMALIZER_H

#include "signal.h"

class Normalizer {
public:
    static Signal normalize(const Signal& signal);
};

#endif