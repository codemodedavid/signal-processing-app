#include "audio_buffer.h"
#include <cstring>

// default constructor (null)
AudioBuffer::AudioBuffer() {
    this->number_of_channels = 0;
    this->number_of_samples = 0;
    channels = nullptr;
}

// constructor for empty AudioBuffer
AudioBuffer::AudioBuffer(int number_of_channels, long number_of_samples) {
    this->number_of_channels = number_of_channels;
    this->number_of_samples = number_of_samples;
    channels = createBuffer(number_of_channels, number_of_samples);
}

// destructor
AudioBuffer::~AudioBuffer() {
    deleteBuffer(channels, number_of_channels);
}

// copy constructor
AudioBuffer::AudioBuffer(const AudioBuffer& other) {
    number_of_channels = other.number_of_channels;
    number_of_samples = other.number_of_samples;

    channels = createBuffer(number_of_channels, number_of_samples);

    for (int channel_index = 0;
         channel_index < number_of_channels;
         channel_index++)
    {
        memcpy(
            channels[channel_index],
            other.channels[channel_index],
            number_of_samples * sizeof(float)
        );
    }
}

// assignment operator
AudioBuffer& AudioBuffer::operator=(const AudioBuffer& other) {
    if (this == &other) {
        return *this;
    }
    deleteBuffer(channels, number_of_channels);

    number_of_channels = other.number_of_channels;
    number_of_samples = other.number_of_samples;

    channels = createBuffer(number_of_channels, number_of_samples);
    for (int channel_index = 0; channel_index < number_of_channels; channel_index++) {
        memcpy(channels[channel_index], other.channels[channel_index], number_of_samples * sizeof(float));
    }
    return *this;
}

// creates an empty buffer
float** AudioBuffer::createBuffer(int number_of_channels, long number_of_samples) { // creates buffer for the signal data: buffer[channel][samples]

    float** buffer = new float*[number_of_channels];
    for(int channel_index=0; channel_index<number_of_channels; channel_index++) {
        buffer[channel_index] = new float[number_of_samples];
    }
    return buffer;
}

// frees memory for a certain buffer
void AudioBuffer::deleteBuffer(float**& buffer, int number_of_channels) { // deletes the buffer for the signal data
    for(int channel_index = 0; channel_index < number_of_channels; channel_index++){
            delete[] buffer[channel_index];
        }
    delete[] buffer;
    buffer = nullptr;
}

// getters
int AudioBuffer::getChannelCount() const {
    return number_of_channels;
}

long AudioBuffer::getSampleCount() const {
    return number_of_samples;
}

// setters
float AudioBuffer::getSample(int channel_index, long sample_index) const {
    return channels[channel_index][sample_index];
}
void AudioBuffer::setSample(int channel_index, long sample_index, float new_value) {
    channels[channel_index][sample_index] = new_value;
    return;
}