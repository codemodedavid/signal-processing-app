#include "signal_data.h"


// ctors
SignalData::SignalData() {

};

SignalData::SignalData(const unsigned char* header) {
    memcpy(riff, header + 0, 4);
    memcpy(&fileSizeMinus8, header + 4, 4);
    memcpy(wave, header + 8, 4);
    memcpy(fmt, header + 12, 4);
    memcpy(&formatChunkSize, header + 16, 4);
    memcpy(&audioFormat, header + 20, 2);
    memcpy(&channels, header + 22, 2);
    memcpy(&sampleRate, header + 24, 4);
    memcpy(&byteRate, header + 28, 4);
    memcpy(&blockAlign, header + 32, 2);
    memcpy(&bitsPerSample, header + 34, 2);
    memcpy(data, header + 36, 4);
    memcpy(&dataSize, header + 40, 4);

    sampleCount=dataSize / blockAlign;
};

// setters
void SignalData::setDataSize(long dataSize) {
    this->dataSize = dataSize;
};
void SignalData::setSampleRate(long sampleRate) {
    this->sampleRate = sampleRate;
};
void SignalData::setSampleCount(long sampleCount) {
    this->sampleCount = sampleCount;
};

// getters
long SignalData::getDataSize() const {
    return dataSize;
};
long SignalData::getSampleRate() const {
    return sampleRate;
};
long SignalData::getSampleCount() const {
    return sampleCount;
};

int SignalData::getBitsPerSample() const {
    return bitsPerSample;
}
int SignalData::getBytesPerSample() const {
    return bitsPerSample/8;
}
int SignalData::getChannels() const {
    return channels;
}

int SignalData::getBlockAlign() const {
    return blockAlign;
}

unsigned char* SignalData::getHeader() const {
    unsigned char* header = new unsigned char[44];
    memcpy(header + 0, riff, 4);
    memcpy(header + 4, &fileSizeMinus8, 4);
    memcpy(header + 8, wave, 4);
    memcpy(header + 12, fmt, 4);
    memcpy(header + 16, &formatChunkSize, 4);
    memcpy(header + 20, &audioFormat, 2);
    memcpy(header + 22, &channels, 2);
    memcpy(header + 24, &sampleRate, 4);
    memcpy(header + 28, &byteRate, 4);
    memcpy(header + 32, &blockAlign, 2);
    memcpy(header + 34, &bitsPerSample, 2);
    memcpy(header + 36, data, 4);
    memcpy(header + 40, &dataSize, 4);

    return header;
}
// afisare
void SignalData::printHeaderData() const {
    std::cout << "riff: " << std::string(riff, 4) << "\n";
    std::cout << "fileSizeMinus8: " << fileSizeMinus8 << "\n";
    std::cout << "wave: " << std::string(wave, 4) << "\n";
    std::cout << "fmt: " << std::string(fmt, 4) << "\n";
    std::cout << "formatChunkSize: " << formatChunkSize << "\n";
    std::cout << "audioFormat: " << audioFormat << "\n";
    std::cout << "channels: " << channels << "\n";
    std::cout << "sampleRate: " << sampleRate << "\n";
    std::cout << "byteRate: " << byteRate << "\n";
    std::cout << "blockAlign: " << blockAlign << "\n";
    std::cout << "bitsPerSample: " << bitsPerSample << "\n";
    std::cout << "data: " << std::string(data, 4) << "\n";
    std::cout << "dataSize: " << dataSize << "\n";
    std::cout << "sampleCount: " << sampleCount << "\n";
};
