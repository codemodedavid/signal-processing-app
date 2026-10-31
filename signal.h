#ifndef SIGNAL_H

#define SIGNAL_H

#include "signal_data.h"
#include "audio_buffer.h"
#include <cstdint>
#include <cstdio>
#include <cmath>

class Signal {
    char* name;
    SignalData data;
    AudioBuffer buffer;
public:
    Signal();
    Signal(int number_of_channels, long number_of_samples);
    Signal(const char* name);
    ~Signal();
    Signal(const Signal& other);
    
    Signal& operator=(const Signal& other);
    Signal operator*(const Signal& other) const;

    void setDataSize(long audioSize);
    void setSampleRate(long sampleRate);
    void setSampleCount(long sampleCount);
    void setChannels(int num_channels);

    long getDataSize() const;
    long getSampleRate() const;
    long getSampleCount() const;
    int getBitsPerSample() const;
    int getBytesPerSample() const;
    int getChannels() const;
    int getBlockAlign() const;
    float getMaxValue() const;

    const SignalData& getSignalData() const;
    const AudioBuffer& getAudioBuffer() const;

    void setSignalData(SignalData& data);
    void setAudioBuffer(AudioBuffer& buffer);


    float getSample(int channel, long number) const;
    void setSample(int channel, long number, float new_value);

    void printDataSample(uint32_t start, uint32_t finish) const;
    void printDataTime(float start, float finish) const;
};

#endif