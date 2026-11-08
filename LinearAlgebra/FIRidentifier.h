#ifndef FIRIDENTIFIER_H
#define FIRIDENTIFIER_H

#include "../signal.h"

class FIRIdentifier {
    Signal filter;
public:
    FIRIdentifier(const Signal& input, const Signal& output, int filter_coefficients);
    Signal getFilter() const;
};

#endif