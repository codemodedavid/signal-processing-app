#include "signal.h"
#include "resampler.h"
#include "wav.h"
#include <cstdint>

int main() {
    Signal s1("semnal.wav");
    Signal s2("IR1.wav");

    std::cout << "Created signals\n";

    Signal s3 = s1 * s2;

    Wav::saveWav("semnal_out.wav", s3);
    return 0;
}