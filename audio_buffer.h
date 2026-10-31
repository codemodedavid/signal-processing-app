#ifndef AUDIOBUFFER_H
#define AUDIOBUFFER_H

class AudioBuffer {
private:
    float** channels;
    int number_of_channels;
    long number_of_samples;

    float** createBuffer(int number_of_channels, long number_of_samples);
    void deleteBuffer(float**& buffer, int number_of_channels);
public:
    AudioBuffer();
    AudioBuffer(int number_of_channels, long number_of_samples);
    ~AudioBuffer();

    AudioBuffer(const AudioBuffer& other);
    AudioBuffer& operator=(const AudioBuffer& other);

    int getChannelCount() const;
    long getSampleCount() const;

    float getSample(int channel_index, long sample_index) const;
    void setSample(int channel_index, long sample_index, float new_value);
};

#endif