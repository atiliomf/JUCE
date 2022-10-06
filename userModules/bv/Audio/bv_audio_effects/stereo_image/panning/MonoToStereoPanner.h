
#pragma once

#include "PannerBase.h"

namespace bav::dsp::FX
{
template < typename SampleType >
class MonoToStereoPanner : public PannerBase
{
public:
    using AudioBuffer = juce::AudioBuffer< SampleType >;

    void prepare (double sr, int blocksize);
    void reset();

    void process (const AudioBuffer& monoInput,
                  AudioBuffer&       stereoOutput);

private:
    SmoothedGain< SampleType, 1 > left, right;
};

}  // namespace bav::dsp::FX
