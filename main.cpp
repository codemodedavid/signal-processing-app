#include "signal.h"
#include <cstdint>

int main() {
    
    float start = 1.0021;
    float finish = 1.0022;

    Signal s1("semnal.wav");
    

    s1.printDataTime(start, finish); // debugging
    
    s1.resample(48000);
    printf("--------------------------RESAMPLED--------------------------\n");
    
    s1.printDataTime(start, finish); // debugging

    s1.saveWav("semnal_out.wav");
    return 1;
}