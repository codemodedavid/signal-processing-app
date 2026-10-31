#include "resampler.h"
#include "signal.h"

// reconstruction with sinc interpolation centered in center on interval [-vecinity, +vecinity] at a moment of time
float* Resampler::reconstruct(const Signal& signal, long center, long vecinity, float time) { // reconstructs singal needed for resampling, in order to compute values at given sample rate

    float* value = new float[signal.getChannels()]();
    float Ts = 1.0f / signal.getSampleRate();

    long start = std::max(0L, center - vecinity);
    long end = std::min(signal.getSampleCount(), center + vecinity + 1);

    for(int channel_index=0; channel_index<signal.getChannels(); channel_index++) {
        for(int sample_index=start; sample_index<end; sample_index++) {
            float arg = (time - sample_index * Ts) / Ts;
            float sinc;

            if(fabs(arg) < 1e-8) {
                sinc = 1.0f;
            }
            else {
                sinc = sin(M_PI * arg) / (M_PI * arg);
            }
            value[channel_index] += signal.getSample(channel_index, sample_index) * sinc;
        }
    }
    return value;
}

// resamples any signal to a certain bitrate using reconstruction with sinc interpolation
Signal Resampler::resample(const Signal& signal, float new_sampling_rate) {
    std::cout << "Resampling\n";
    std::cout << 1.0f * signal.getSampleCount() / signal.getSampleRate() << "\n";

    long old_sampling_rate = signal.getSampleRate();
    if(std::abs((old_sampling_rate - new_sampling_rate) / new_sampling_rate) <= 0.001) {
        return Signal();
    }
    long new_sample_count = static_cast<long>(signal.getSampleCount() * new_sampling_rate / old_sampling_rate);

    Signal new_signal(signal);
    AudioBuffer new_buffer(signal.getChannels(), new_sample_count);

    for(long sample_count=0; sample_count<new_sample_count; sample_count++) {

        float time = static_cast<float>(sample_count) / new_sampling_rate;
        long center = static_cast<long>(time * signal.getSampleRate());

        float* updated_samples = reconstruct(signal, center, 100, time);

        for(int channel_index=0; channel_index<signal.getChannels(); channel_index++) {
            new_buffer.setSample(channel_index, sample_count, updated_samples[channel_index]);
        }
        delete[] updated_samples;
    }
    new_signal.setAudioBuffer(new_buffer);

    new_signal.setSampleRate(new_sampling_rate);
    new_signal.setSampleCount(new_sample_count);
    new_signal.setDataSize(new_sample_count * new_signal.getChannels() * (new_signal.getBytesPerSample()));

    std::cout << 1.0f * new_signal.getSampleCount() / new_signal.getSampleRate() << "\n";
    return new_signal;
}