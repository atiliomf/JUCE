
#pragma once

namespace bav::dsp::FX
{
template < typename SampleType >
class StereoPanner : public PannerBase, public AudioEffect< SampleType >
{
public:
    using AudioBuffer = juce::AudioBuffer< SampleType >;

    void prepare (double samplerate, int blocksize) final;
    void process (AudioBuffer& audio) final;

    void reset();

    void process (const AudioBuffer& stereoInput,
                  AudioBuffer&       stereoOutput);

private:
    SmoothedGain< SampleType, 1 > left, right;
};

}  // namespace bav::dsp::FX
