#include "signal.h"
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>


// constructor + init
Signal::Signal(int number_of_channels, long number_of_samples) { // init empty signal with nr. of channels and samples
    name = nullptr;
    
    data = nullptr;

    channel = new float*[number_of_channels];
    for(int i=0; i<number_of_channels; i++) {
        channel[i] = new float[number_of_samples];
    }
};

Signal::Signal(const char* name) { // init a signal based on a given .wav file path
    this->name = new char[strlen(name) -4 + 1 ];
    int i;
    for(i=0; i<strlen(name); i++) {
        if(name[i] == '.') {
            break;
        } else {
            this->name[i] = name[i];
        }
    }
    this->name[i] = '\0';
    // opening file by path
    FILE* file_in = fopen(name, "rb");
    if(!file_in) {
        printf("Cannot open signal A\n");
        return;
    }
    
    // allocating header to pass into SignalData constructor
    unsigned char* header =new unsigned char[44];
    fread(header, 1, 44, file_in);

    this->data = SignalData(header);
    delete[] header;

    // printing header details for debugging
    this->data.printHeaderData();

    // starting signal processing
    float (*decode)(const char*) = nullptr;

    switch (data.getBitsPerSample()) {
        case 8:
            decode = Signal::decode8;
            break;
        case 16:
            decode = Signal::decode16;
            break;
        case 32:
            decode = Signal::decode32;
            break;
        default:
            printf("unsupported bit depth\n");
            return;
    }

    channel = new float*[data.getChannels()];
    for(int i=0; i<data.getChannels(); i++) {
        channel[i] = new float[data.getSampleCount()];
    }
    char* frame = new char[data.getBlockAlign()];
    

    for(size_t sampleIndex=0; sampleIndex < data.getSampleCount(); sampleIndex++) {
        fread(frame, 1, data.getBlockAlign(), file_in);
        for(int ch=0; ch<data.getChannels(); ch++) {
            size_t offset = ch * data.getBytesPerSample();
            float sample = decode(frame + offset);
            
            channel[ch][sampleIndex] = sample;
        }
    }
    delete[] frame;
    fclose(file_in);
};

// destructor + free
Signal::~Signal() {
    delete[] name;
    name = nullptr;

    for(int i = 0; i < data.getChannels(); i++)
    {
        delete[] channel[i];
    }
    delete[] channel;
    channel = nullptr;
};

// copy constructor
Signal::Signal(const Signal& other) {
    name = new char[strlen(other.name) + 1];
    strcpy(name, other.name);
    data = other.data;
    channel = new float*[data.getChannels()];
    for(int i=0; i<data.getChannels(); i++) {
        channel[i] = new float[data.getSampleCount()];
        memcpy(channel[i], other.channel[i], data.getSampleCount() * sizeof(float));
    }
};

// convolution operator
// Signal& Signal::operator*(const Signal& other) {
//     return nullptr;
// };

float* Signal::reconstruct(long center, long vecinity, float time) const { // reconstructs singal needed for resampling, in order to compute values at given sample rate

    float* value = new float[data.getChannels()]();
    float Ts = 1.0 / data.getSampleRate();

    long start = std::max(0L, center - vecinity);
    long end = std::min(data.getSampleCount(), center + vecinity + 1);

    for(int i=0; i<data.getChannels(); i++) {
        for(int n=start; n<end; n++) {
            float arg = (time - n * Ts) / Ts;
            float sinc;

            if(fabs(arg) < 1e-8) {
                sinc = 1.0;
            }
            else {
                sinc = sin(M_PI * arg) / (M_PI * arg);
            }
            value[i] += channel[i][n] * sinc;
        }
    }
    return value;
};

float** createBuffer(int number_of_channels, long number_of_samples) { // creates buffer for the signal data: buffer[channel][samples]

    float** buffer = new float*[number_of_channels];
    for(int i=0; i<number_of_channels; i++) {
        buffer[i] = new float[number_of_samples];
    }
    return buffer;
};

Signal& Signal::resample(float new_sampling_rate) { // resamples any signal to a certain bitrate using reconstruction using sinc interpolation 

    std::cout << 1.0f * data.getSampleCount() / data.getSampleRate() << "\n";

    long old_sampling_rate = data.getSampleRate();
    if(std::abs((old_sampling_rate - new_sampling_rate) / new_sampling_rate) <= 0.001) {
        return *this;
    }
    long new_sample_count = static_cast<long>(data.getSampleCount() * new_sampling_rate / old_sampling_rate);

    float** buff_channel = createBuffer(data.getChannels(), new_sample_count);
    double Ts = 1.0 / new_sampling_rate;
    double time = 0.0;

    for(long sample_count=0; sample_count<new_sample_count; sample_count++) {

        float time = static_cast<float>(sample_count) / new_sampling_rate;
        long center = static_cast<long>(time * data.getSampleRate());

        float* updated_samples = reconstruct(center, 100, time);

        for(int c=0; c<data.getChannels(); c++) {
            buff_channel[c][sample_count] = updated_samples[c];
            //buff_channel[c][sample_count] = sin(time*M_PI*40)/2;
        }

        delete[] updated_samples;
    }

    for(int i = 0; i < data.getChannels(); i++)
    {
        delete[] channel[i];
    }
    delete[] channel;
    channel = nullptr;

    channel = buff_channel;

    data.setSampleRate(new_sampling_rate);
    data.setSampleCount(new_sample_count);
    data.setDataSize(new_sample_count * data.getChannels()
    * (data.getBitsPerSample()/ 8));

    std::cout << 1.0f * data.getSampleCount() / data.getSampleRate() << "\n";
    //data.printHeaderData(); //debugging
    return *this;
};

void Signal::saveWav(const char* name) {

    FILE* file_out = fopen(name, "wb");
    unsigned char* header = data.getHeader();
    fwrite(header, 1, 44, file_out);
    delete[] header;
    switch (data.getBitsPerSample()) {
        case 8:
            write8(file_out);
            break;
        case 16:
            write16(file_out);
            break;
        case 32:
            write32(file_out);
            break;
        default:
            printf("unable to write\n");
            return;
    }
    fclose(file_out);
}

// decoding-encoding functions int-float, float-int respectively
float Signal::decode8(const char* data) {
    uint8_t value;
    memcpy(&value, data, sizeof(value));

    return (value - 128) / 128.0f;
};

float Signal::decode16(const char* data) {
    int16_t value;
    memcpy(&value, data, sizeof(value));

    return value / 32768.0f;
};

float Signal::decode32(const char* data) {
    int32_t value;
    memcpy(&value, data, sizeof(value));

    return value / 2147483648.0f;
};

uint8_t Signal::encode8(float data) {
    return static_cast<uint8_t>(data * 128.0f + 128);
};

int16_t Signal::encode16(float data) {
    return static_cast<int16_t>(data * 32768.0f);
};

int32_t Signal::encode32(float data) {
    return static_cast<int32_t>(data * 2147483648.0f);
};

void Signal::write8(FILE* file_out) {
    for (long i = 0; i < data.getSampleCount(); i++) {
        for (int ch = 0; ch < data.getChannels(); ch++) {
            uint8_t value = encode8(channel[ch][i]);
            fwrite(&value, sizeof(value), 1, file_out);
        }
    }
}

void Signal::write16(FILE* file_out) {
    for (long i = 0; i < data.getSampleCount(); i++) {
        for (int ch = 0; ch < data.getChannels(); ch++) {
            int16_t value = encode16(channel[ch][i]);
            fwrite(&value, sizeof(value), 1, file_out);
        }
    }
}

void Signal::write32(FILE* file_out) {
    for (long i = 0; i < data.getSampleCount(); i++) {
        for (int ch = 0; ch < data.getChannels(); ch++) {
            int32_t value = encode32(channel[ch][i]);
            fwrite(&value, sizeof(value), 1, file_out);
        }
    }
}
// printing
void Signal::printDataSample(uint32_t start, uint32_t finish) const {
    if(start < 0) {
        start=0;
    }
    if (finish > data.getSampleCount()) {
        finish = data.getSampleCount();
    }
    data.printHeaderData();
    for(int i=start; i< finish; i++) {
        std::cout << channel[0][i] << "\n";
    }
};

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
    for(int i=start_index; i<finish_index; i++) {
        std::cout << channel[0][i]<< "\n";
    }
};

