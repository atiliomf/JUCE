
#pragma once

namespace bav::dsp::FX
{
template < typename SampleType, size_t channels = 1 >
class SmoothedGain : public AudioEffect< SampleType >
{
public:
    using AudioBuffer = juce::AudioBuffer< SampleType >;

    SmoothedGain();

    void prepare (double samplerate, int blocksize) final;
    void process (AudioBuffer& audio) final;

    void setGain (float gain);

    void reset();
    void skipSamples (int numSamples);

private:
    juce::OwnedArray< ValueSmoother< SampleType > > smoothers;

    int lastBlocksize = 0;
};

}  // namespace bav::dsp::FX
