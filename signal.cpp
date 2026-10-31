#include "signal.h"
#include "signal_data.h"
#include "audio_buffer.h"
#include "wav.h"
#include "resampler.h"
#include "normalizer.h"

#include <cmath>
#include <cstdint>
#include <cstring>

// default constructor (null)
Signal::Signal() {
    name = nullptr;
    data = SignalData();
    buffer = AudioBuffer();
}

// constructor for empty signal
Signal::Signal(int number_of_channels, long number_of_samples) { // init empty signal with nr. of channels and samples
    name = nullptr;
    data = SignalData();
    buffer = AudioBuffer(number_of_channels, number_of_samples);
}

// constructor for .wav signal
Signal::Signal(const char* name) { // init a signal based on a given .wav file path
    this->name = new char[strlen(name) - 4 + 1 ];
    for(int i=0; i<strlen(name); i++) {
        if(name[i] == '.') {
            this->name[i] = '\0';
            break;
        } else {
            this->name[i] = name[i];
        }
    }
    // opening file by path
    data = Wav::createSignalData(name);
    buffer = Wav::createAudioBuffer(name, data);
}

// destructor + free
Signal::~Signal() {
    delete[] name;
    name = nullptr;
}

// copy constructor
Signal::Signal(const Signal& other) {
    name = new char[strlen(other.name) + 1];
    strcpy(name, other.name);
    data = other.data;
    buffer = other.buffer;
}

// assignment operator
Signal& Signal::operator=(const Signal& other) {
    if (this == &other)
        return *this;
    delete[] name;
    name = new char[strlen(other.name) + 1];
    strcpy(name, other.name);
    data = other.data;
    buffer = other.buffer;
    return *this;
}

// convolution operator (naive)
Signal Signal::operator*(const Signal& other) const {
    Signal s2(other); // copy for second signal and we resample it if it has diffrent frenquency
    if(this->data.getSampleRate() != s2.data.getSampleRate()){
        Signal resampled = Resampler::resample(s2, this->data.getSampleRate());
    }

    long new_sample_count = this->data.getSampleCount() + s2.data.getSampleCount() - 1;
    int num_channels = std::max(this->data.getChannels(), s2.data.getChannels());

    // initialize result
    Signal result(*this);
    result.data.setSampleCount(new_sample_count);
    result.data.setDataSize(new_sample_count * num_channels * result.data.getBytesPerSample());
    result.data.setChannels(num_channels);

    // made a new buffer for the result of convolution
    AudioBuffer result_buffer(result.getChannels(), result.getSampleCount());
    result.buffer = result_buffer;

    // convolution formula ( the basic one) y[i + j] = x[i] * h[j]
    std::cout << "Starting convolution\n";

    for(int ch = 0; ch < num_channels; ch++) {
        int ch_s1;
        int ch_s2;
        if (this->data.getChannels() == 1) {
            ch_s1 = 0;
        } else {
            ch_s1 = ch;
        }
        if (s2.data.getChannels() == 1) {
            ch_s2 = 0;
        } else {
            ch_s2 = ch;
        }

        for (long i = 0; i < this->data.getSampleCount(); i++) {
            for (long j = 0; j < s2.data.getSampleCount(); j++) {
                float new_value = result.buffer.getSample(ch, i + j) + this->buffer.getSample(ch_s1, i) * s2.buffer.getSample(ch_s2, j);
                result.buffer.setSample(ch, i + j, new_value);
            }
        }
    }
    std::cout << "Finished convolution\n";
    // normalize the signal in [-1.0, 1.0] so that it does not distort when save in saveWav
    result = Normalizer::normalize(result);
    
    return result;
}

// setters
void Signal::setDataSize(long audioSize) {
    data.setDataSize(audioSize);
}
void Signal::setSampleRate(long sampleRate) {
    data.setSampleRate(sampleRate);
}
void Signal::setSampleCount(long sampleCount) {
    data.setSampleCount(sampleCount);
}
void Signal::setChannels(int num_channels) {
    data.setChannels(num_channels);
}

// getters
long Signal::getDataSize() const {
    return data.getDataSize();
}
long Signal::getSampleRate() const {
    return data.getSampleRate();
}
long Signal::getSampleCount() const {
    return data.getSampleCount();
}
int Signal::getBitsPerSample() const {
    return data.getBitsPerSample();
}
int Signal::getBytesPerSample() const {
    return data.getBitsPerSample()/8;
}
int Signal::getChannels() const {
    return data.getChannels();
}
int Signal::getBlockAlign() const {
    return data.getBlockAlign();
}

// returns the SignalData object
const SignalData& Signal::getSignalData() const {
    return data;
}
// returns the AudioBuffer object
const AudioBuffer& Signal::getAudioBuffer() const {
    return buffer;
}

// sets the SignalData object
void Signal::setSignalData(SignalData& data) {
    this->data = data;
}
// sets the AudioBuffer object
void Signal::setAudioBuffer(AudioBuffer& buffer) {
    this->buffer = buffer;
}

// gets a sample in AudioBuffer
float Signal::getSample(int channel_index, long sample_index) const {
    return buffer.getSample(channel_index, sample_index);
}
// sets a new value to a sample in AudioBuffer
void Signal::setSample(int channel_index, long sample_index, float new_value) {
    return buffer.setSample(channel_index, sample_index, new_value);
}

// return the max value of the signal as float
float Signal::getMaxValue() const {
    float max_val = 0.0f;
    for (int channel_index = 0; channel_index < data.getChannels(); channel_index++) {
        for (long sample_index=0; sample_index<data.getSampleCount(); sample_index++) {
            if (std::abs(this->getSample(channel_index, sample_index)) > max_val) {
                max_val = std::abs(this->getSample(channel_index, sample_index));
            }
        }
    }
    return max_val;
}

// prints data samples in a certain sample count interval
void Signal::printDataSample(uint32_t start, uint32_t finish) const {
    if(start < 0) {
        start=0;
    }
    if (finish > data.getSampleCount()) {
        finish = data.getSampleCount();
    }
    data.printHeaderData();
    for(int sample_index=start; sample_index< finish; sample_index++) {
        std::cout << buffer.getSample(0, sample_index) << "\n";
    }
}

// prints data samples in a certain time interval
void Signal::printDataTime(float start, float finish) const {

    if(start < 0.0f) {
        start=0.0f;
    }
    if (finish > 1.0f * data.getSampleCount() / data.getSampleRate()) {
        finish = 1.0f * data.getSampleCount() / data.getSampleRate();
    }

    data.printHeaderData();

    int32_t start_index = static_cast<int32_t>(start * data.getSampleRate());
    int32_t finish_index = static_cast<int32_t>(finish * data.getSampleRate());
    for(int sample_index=start; sample_index< finish; sample_index++) {
        std::cout << buffer.getSample(0, sample_index) << "\n";
    }
}

