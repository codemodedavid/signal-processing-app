#include "signal.h"
#include <cstdio>
#include <cstring>

// constructor + init
Signal::Signal(const char* name) {
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
        printf("Cannot open signal A");
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

void Signal::printData() {
    for(int i=0; i< data.getSampleCount(); i++) {
        std::cout << channel[0][i]<< "\n";
    }
}
