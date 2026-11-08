#include "FIRidentifier.h"
#include "../audio_buffer.h"
#include "matrix.h"
#include "QR.h"

FIRIdentifier::FIRIdentifier(const Signal& input, const Signal& output, int filter_coefficients) {

    Signal filter(input);
    AudioBuffer filter_buffer(input.getChannels(), filter_coefficients);

    for(long channel_index = 0; channel_index<input.getChannels(); channel_index++) {

        Matrix X(input.getSampleCount(), filter_coefficients);

        for(long row_index=0; row_index<input.getSampleCount(); row_index++) {
            for(int column_index=0; column_index<filter_coefficients; column_index++) {
                if(row_index >= column_index) {
                    X[row_index][column_index] = input.getSample(channel_index, row_index - column_index);
                }
                else {
                    X[row_index][column_index] = 0.0f;
                }
            }
        }
        Matrix aux_out(output.getSampleCount(), 1); // desired signal / output

        for(long sample_index=0; sample_index<output.getSampleCount(); sample_index++) {
            aux_out[sample_index][0] = output.getSample(channel_index, sample_index);
        }

        QR qr(X);
        Matrix result = qr.solve(aux_out);

        for(int coefficient_index=0; coefficient_index<filter_coefficients; coefficient_index++) {
            filter_buffer.setSample(channel_index, coefficient_index, result[coefficient_index][0]);
        }
    }

    filter.setAudioBuffer(filter_buffer);
    filter.setSampleCount(filter_coefficients);
    filter.setDataSize(filter_coefficients * filter.getChannels() * filter.getBytesPerSample());
    this->filter = filter;
}

Signal FIRIdentifier::getFilter() const {
    return filter;
}

