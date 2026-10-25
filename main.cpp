#include "signal.h"
#include <cstdint>

int main() {
    Signal s1("semnal.wav");
    Signal s2("IR3.wav");

    //float start = 1.0021;
    //float finish = 1.0022;
    //s1.printDataTime(start, finish); // debugging
    
    //s1.resample(30000);
    //printf("--------------------------RESAMPLED--------------------------\n");
    //s1.printDataTime(start, finish); // debugging
    //s1.saveWav("semnal_out.wav");

    Signal s3 = s1 * s2;
    s3.saveWav("semnal_out.wav");
    return 0;
}