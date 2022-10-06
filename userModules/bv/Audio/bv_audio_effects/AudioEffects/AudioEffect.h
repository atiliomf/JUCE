#pragma once

#include <bv_dsp/bv_dsp.h>

namespace bav::dsp::FX
{
template < typename SampleType >
struct AudioEffect
{
    virtual ~AudioEffect() = default;

    virtual void process (juce::AudioBuffer< SampleType >& audio) = 0;

    virtual void prepare (double samplerate, int blocksize);
    virtual void bypassedBlock (int numSamples);
};


template < typename SampleType >
struct SidechainableAudioEffect : AudioEffect< SampleType >
{
    using AudioBuffer = juce::AudioBuffer< SampleType >;

    virtual void process (AudioBuffer& inOut, const AudioBuffer& sidechain) = 0;

    /* Sidechains the signal to itself. */
    void process (AudioBuffer& audio) override;
};


/* An effect that reports either its level or its gain reduction for each channel */
template < typename SampleType >
struct LevelReportingAudioEffect : SidechainableAudioEffect< SampleType >
{
    using AudioBuffer = juce::AudioBuffer< SampleType >;

    void process (AudioBuffer& inOut, const AudioBuffer& sidechain) final;

    /* Sidechains the signal to itself. */
    void process (AudioBuffer& audio) final;

    /* Should return the channel's average level / gain reduction for this frame */
    virtual SampleType processChannel (int               channel,
                                       int               numSamples,
                                       SampleType*       signal,
                                       const SampleType* sidechain) = 0;

    SampleType getGainReduction (int channel) const;
    SampleType getAverageGainReduction() const;
    SampleType getLevel (int channel) const;
    SampleType getAverageLevel() const;

private:
    juce::Array< SampleType > gainReductions;
};

}  // namespace bav::dsp::FX
