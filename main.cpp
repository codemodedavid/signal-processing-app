#include "audio_buffer.h"
#include "signal.h"
#include "resampler.h"
#include "wav.h"
#include <cstdint>

#include "LinearAlgebra/QR.h"
#include "LinearAlgebra/matrix.h"
#include "LinearAlgebra/FIRidentifier.h"

int main() {
    Signal s1("semnal.wav");
    Signal s3("semnal_out.wav");

    AudioBuffer s3_buffer(s1.getChannels(), s1.getSampleCount());
    for(int j=0; j<s1.getChannels(); j++) {
        for(int i=0; i<s1.getSampleCount(); i++) {
            s3_buffer.setSample(j, i, s3.getSample(j, i));
        }
    }
    s3.setSampleCount(s1.getSampleCount());
    s3.setAudioBuffer(s3_buffer);
    FIRIdentifier fir(s1, s3, 64);
    Signal filter = fir.getFilter();
    Wav::saveWav("filter.wav", filter);

    Signal s4 = s1 * filter;
    Wav::saveWav("filtered.wav", s4);
    for(long i = 0; i < filter.getSampleCount(); i++) {
    std::cout << i << ": " << filter.getSample(0, i) << "\n";
    }
}