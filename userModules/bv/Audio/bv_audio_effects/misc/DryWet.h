#pragma once

namespace bav::dsp::FX
{
template < typename SampleType >
class DryWetMixer
{
public:
    using AudioBuffer = juce::AudioBuffer< SampleType >;

    DryWetMixer();

    void setWetMix (int wetPercent);
    void setWetLatency (int latencySamples);

    void prepare (int numChannels, int blocksize, double samplerate);
    void reset();

    void pushDrySamples (AudioBuffer& buffer);
    void mixWetSamples (AudioBuffer& buffer);

    void process (AudioBuffer& dry, AudioBuffer& wet);

private:
    juce::dsp::DryWetMixer< SampleType > mixer {512};
    juce::dsp::ProcessSpec               spec;
};

}  // namespace bav::dsp::FX
