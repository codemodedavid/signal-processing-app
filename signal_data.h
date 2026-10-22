#ifndef SIGNAL_DATA_H

#define SIGNAL_DATA_H

#include <cstdint>
#include <iostream>
#include <cstring>

class SignalData {
    // .WAV file HEADER (standard)
    // Offset 0
    char     riff[4];
    // Offset 4
    uint32_t fileSizeMinus8;
    // Offset 8
    char     wave[4];
    // Offset 12
    char     fmt[4];
    // Offset 16
    uint32_t formatChunkSize;
    // Offset 20
    uint16_t audioFormat;
    // Offset 22
    uint16_t channels;
    // Offset 24
    uint32_t sampleRate;
    // Offset 28
    uint32_t byteRate;
    // Offset 32
    uint16_t blockAlign;
    // Offset 34
    uint16_t bitsPerSample;
    // Offset 36
    char     data[4];
    // Offset 40
    uint32_t dataSize;

    long sampleCount;
public:
    SignalData();
    SignalData(const unsigned char* header);

    void setDataSize(long audioSize);
    void setSampleRate(long sampleRate);
    void setSampleCount(long sampleCount);

    long getDataSize() const;
    long getSampleRate() const;
    long getSampleCount() const;
    int getBitsPerSample() const;
    int getBytesPerSample() const;
    int getChannels() const;
    int getBlockAlign() const;

    void printHeaderData() const;
};
#endif