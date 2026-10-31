#ifndef WAV_H
#define WAV_H

#include "audio_buffer.h"
#include "signal_data.h"
#include "signal.h"

class Wav {
private:
    static float decode8(const char* data);
    static float decode16(const char* data);
    static float decode24(const char* data);
    static float decode32(const char* data);
    static void wav_to_buffer8(FILE* file, AudioBuffer& buffer, int block_align, int bytes_per_sample);
    static void wav_to_buffer16(FILE* file, AudioBuffer& buffer, int block_align, int bytes_per_sample);
    static void wav_to_buffer24(FILE* file, AudioBuffer& buffer, int block_align, int bytes_per_sample);
    static void wav_to_buffer32(FILE* file, AudioBuffer& buffer, int block_align, int bytes_per_sample);

    static uint8_t encode8(float data);
    static int16_t encode16(float data);
    static int32_t encode24(float data);
    static int32_t encode32(float data);
    static void buffer_to_wav8(FILE* file, const AudioBuffer& buffer);
    static void buffer_to_wav16(FILE* file, const AudioBuffer& buffer);
    static void buffer_to_wav24(FILE* file, const AudioBuffer& buffer);
    static void buffer_to_wav32(FILE* file, const AudioBuffer& buffer);
public:
    static SignalData createSignalData(const char* name);
    static AudioBuffer createAudioBuffer(const char* name, const SignalData& data);
    static void saveWav(const char* name, const Signal& signal);
};

#endif