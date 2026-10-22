#include "signal.h"

int main() {
    
    Signal s1("semnal.wav");
    //s1.printData(); // debugging
    
    s1.resample(48000);

    //s1.printData(); // debugging
    return 1;
}