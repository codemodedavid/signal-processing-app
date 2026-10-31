#include "normalizer.h"

// performs max-abs normalization to a certain signal
Signal Normalizer::normalize(const Signal& signal) {
    Signal normalized(signal);
    float max_value = normalized.getMaxValue();
    if(max_value > 1.0f) {
        for (int ch = 0; ch < normalized.getChannels(); ch++) {
            for (long smp=0; smp<normalized.getSampleCount(); smp++) {
                float new_value = normalized.getSample(ch, smp) / max_value;
                normalized.setSample(ch, smp, new_value);
            }
        }
    }
    return normalized;
}