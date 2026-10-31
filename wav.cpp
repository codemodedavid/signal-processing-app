#include "wav.h"
#include "audio_buffer.h"

// creates SignalData based on .wav data
SignalData Wav::createSignalData(const char* name) {
    FILE* file_in = fopen(name, "rb");
    if(!file_in) {
        printf("Cannot open signal A\n");
        return SignalData();
    }
    // allocating header to pass into SignalData constructor
    unsigned char* header =new unsigned char[44];
    fread(header, 1, 44, file_in);
    SignalData data(header);

    delete[] header;
    fclose(file_in);
    return data;
}

// creates AudioBuffer based on .wav data
AudioBuffer Wav::createAudioBuffer(const char* name, const SignalData& data) {
    FILE* file_in = fopen(name, "rb");
    if(!file_in) {
        printf("Cannot open signal A\n");
        return AudioBuffer();
    }
    std::cout << "Creating audio buffer\n";
    // skip header
    fseek(file_in, 44, SEEK_SET); 
    AudioBuffer buffer(data.getChannels(), data.getSampleCount());
    int block_align = data.getBlockAlign();
    int bytes_per_sample = data.getBytesPerSample();
    switch (data.getBitsPerSample()) {
        case 8:
            wav_to_buffer8(file_in, buffer, block_align, bytes_per_sample);
            break;
        case 16:
            wav_to_buffer16(file_in, buffer, block_align, bytes_per_sample);
            break;
        case 24:
            wav_to_buffer24(file_in, buffer, block_align, bytes_per_sample);
            break;
        case 32:
            wav_to_buffer32(file_in, buffer, block_align, bytes_per_sample);
            break;
        default:
            printf("unsupported bit depth\n");
            return AudioBuffer();
    }
    fclose(file_in);
    return buffer;
}

// save signal to .wav file format
void Wav::saveWav(const char* name, const Signal& signal) {
    FILE* file_out = fopen(name, "wb");
    std::cout << "Saving audio file\n";
    const SignalData& data = signal.getSignalData();
    const AudioBuffer& buffer = signal.getAudioBuffer();

    unsigned char* header = data.getHeader();
    fwrite(header, 1, 44, file_out);
    delete[] header;
    switch (data.getBitsPerSample()) {
        case 8:
            buffer_to_wav8(file_out, buffer);
            break;
        case 16:
            buffer_to_wav16(file_out, buffer);
            break;
        case 24:
            buffer_to_wav24(file_out, buffer);
        case 32:
            buffer_to_wav32(file_out, buffer);
            break;
        default:
            printf("unable to write\n");
            return;
    }
    fclose(file_out);
}

// 8-bit decoding
float Wav::decode8(const char* data) {
    uint8_t value;
    memcpy(&value, data, sizeof(value));
    return (value - 128) / 128.0f;
}

// 16-bit decoding
float Wav::decode16(const char* data) {
    int16_t value;
    memcpy(&value, data, sizeof(value));
    return value / 32768.0f;
}

// 24-bit decoding
float Wav::decode24(const char* data) {
    int32_t value =
        (static_cast<unsigned char>(data[0])) |
        (static_cast<unsigned char>(data[1]) << 8) |
        (static_cast<unsigned char>(data[2]) << 16);
    if (value & 0x00800000) {
        value |= 0xFF000000;
    }
    return static_cast<float>(value) / 8388608.0f;
}

// 32-bit decoding
float Wav::decode32(const char* data) {
    int32_t value;
    memcpy(&value, data, sizeof(value));
    return value / 2147483648.0f;
}

// writing from .wav to buffer (8-bit decoding)
void Wav::wav_to_buffer8(FILE* file_in, AudioBuffer& buffer, int block_align, int bytes_per_sample) {
    char* frame = new char[block_align];
    for(size_t sample_index=0; sample_index < buffer.getSampleCount(); sample_index++) {
        fread(frame, 1, block_align, file_in);
        for(int channel_index=0; channel_index<buffer.getChannelCount(); channel_index++) {
            size_t offset = channel_index * bytes_per_sample;
            float sample = decode8(frame + offset);
            buffer.setSample(channel_index, sample_index, sample) ;
        }
    }
    delete[] frame;
}

// writing from .wav to buffer (16-bit decoding)
void Wav::wav_to_buffer16(FILE* file_in, AudioBuffer& buffer, int block_align, int bytes_per_sample) {
    char* frame = new char[block_align];
    for(size_t sample_index=0; sample_index < buffer.getSampleCount(); sample_index++) {
        fread(frame, 1, block_align, file_in);
        for(int channel_index=0; channel_index<buffer.getChannelCount(); channel_index++) {
            size_t offset = channel_index * bytes_per_sample;
            float sample = decode16(frame + offset);
            buffer.setSample(channel_index, sample_index, sample) ;
        }
    }
    delete[] frame;
}

// writing from .wav to buffer (24-bit decoding)
void Wav::wav_to_buffer24(FILE* file_in, AudioBuffer& buffer, int block_align, int bytes_per_sample) {
    char* frame = new char[block_align];
    for(size_t sample_index=0; sample_index < buffer.getSampleCount(); sample_index++) {
        fread(frame, 1, block_align, file_in);
        for(int channel_index=0; channel_index<buffer.getChannelCount(); channel_index++) {
            size_t offset = channel_index * bytes_per_sample;
            float sample = decode24(frame + offset);
            buffer.setSample(channel_index, sample_index, sample) ;
        }
    }
    delete[] frame;
}

// writing from .wav to buffer (32-bit decoding)
void Wav::wav_to_buffer32(FILE* file_in, AudioBuffer& buffer, int block_align, int bytes_per_sample) {
    char* frame = new char[block_align];
    for(size_t sample_index=0; sample_index < buffer.getSampleCount(); sample_index++) {
        fread(frame, 1, block_align, file_in);
        for(int channel_index=0; channel_index<buffer.getChannelCount(); channel_index++) {
            size_t offset = channel_index * bytes_per_sample;
            float sample = decode32(frame + offset);
            buffer.setSample(channel_index, sample_index, sample) ;
        }
    }
    delete[] frame;
}

// 8-bit encoding
uint8_t Wav::encode8(float data) {
    return static_cast<uint8_t>(data * 128.0f + 128);
}

// 16-bit encoding
int16_t Wav::encode16(float data) {
    return static_cast<int16_t>(data * 32768.0f);
}

// 24-bit encoding
int32_t Wav::encode24(float data) {
    return static_cast<int32_t>(data * 8388607.0f);
}

// 32-bit encoding
int32_t Wav::encode32(float data) {
    return static_cast<int32_t>(data * 2147483648.0f);
}

// writing from buffer to .wav (8-bit encoding)
void Wav::buffer_to_wav8(FILE* file_out, const AudioBuffer& buffer) {
    for (long sample_index = 0; sample_index < buffer.getSampleCount(); sample_index++) {
        for (int channel_index = 0; channel_index < buffer.getChannelCount(); channel_index++) {
            uint8_t value = encode8(buffer.getSample(channel_index, sample_index));
            fwrite(&value, sizeof(value), 1, file_out);
        }
    }
}

// writing from buffer to .wav (16-bit encoding)
void Wav::buffer_to_wav16(FILE* file_out, const AudioBuffer& buffer) {
    for (long sample_index = 0; sample_index < buffer.getSampleCount(); sample_index++) {
        for (int channel_index = 0; channel_index < buffer.getChannelCount(); channel_index++) {
            int16_t value = encode16(buffer.getSample(channel_index, sample_index));
            fwrite(&value, sizeof(value), 1, file_out);
        }
    }
}

// writing from buffer to .wav (24-bit encoding)
void Wav::buffer_to_wav24(FILE* file_out, const AudioBuffer& buffer) {
    for (long sample_index = 0; sample_index < buffer.getSampleCount(); sample_index++) {
        for (int channel_index = 0; channel_index < buffer.getChannelCount(); channel_index++) {
            int32_t value = encode24(buffer.getSample(channel_index, sample_index));
            unsigned char bytes[3];
            bytes[0] = value & 0xFF;
            bytes[1] = (value >> 8) & 0xFF;
            bytes[2] = (value >> 16) & 0xFF;
            fwrite(bytes, sizeof(bytes), 1, file_out);
        }
    }
}

// writing from buffer to .wav (32-bit encoding)
void Wav::buffer_to_wav32(FILE* file_out, const AudioBuffer& buffer) {
    for (long sample_index = 0; sample_index < buffer.getSampleCount(); sample_index++) {
        for (int channel_index = 0; channel_index < buffer.getChannelCount(); channel_index++) {
            int32_t value = encode32(buffer.getSample(channel_index, sample_index));
            fwrite(&value, sizeof(value), 1, file_out);
        }
    }
}