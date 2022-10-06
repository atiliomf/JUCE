
#pragma once

namespace bav::dsp
{
template < typename SampleType >
class LFO : public osc::ChoosableOscillator< SampleType >
{
public:
    using AudioBuffer = juce::AudioBuffer< SampleType >;

    void process (int numSamples);

private:
    void prepared (int blocksize) final;

    AudioBuffer storage;
};

}  // namespace bav::dsp
