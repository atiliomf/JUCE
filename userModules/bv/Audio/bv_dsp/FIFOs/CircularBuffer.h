
#pragma once

#include <juce_audio_utils/juce_audio_utils.h>

namespace bav::dsp
{
template < typename SampleType >
class CircularBuffer
{
public:
    using AudioBuffer = juce::AudioBuffer< SampleType >;

    void resize (int blocksize, int blocksizeMultipleToAllocate = 4);

    void storeSamples (const AudioBuffer& samples, int channel = 0);
    void storeSamples (const SampleType* samples, int numSamples);

    int getCapacity() const;
    int getLastFrameStartIndex() const;
    int getLastFrameEndIndex() const;

    int clipValueToCapacity (int value) const;

    SampleType getSample (int index) const;

private:
    AudioBuffer buffer;

    int lastFrameStart {0};  // the sample index in the buffer where the first sample of the last frame is stored
    int lastFrameEnd {0};    // the sample index in the buffer where the last sample of the last frame is stored

    int lastFrameSize {0};
};

}  // namespace bav::dsp
