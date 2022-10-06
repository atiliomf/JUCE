
#pragma once

#include <juce_dsp/juce_dsp.h>

namespace bav::dsp::FX
{
/*
        Simple noise gate that allows you to sidechain the signal.
    */

template < typename SampleType >
class NoiseGate : public LevelReportingAudioEffect< SampleType >
{
public:
    NoiseGate();

    void prepare (double samplerate, int blocksize) final;

    void setInverted (bool gateBehaviorShouldBeInverted);
    void setThreshold (float newThreshold_dB);
    void setRatio (SampleType newRatio);
    void setAttack (SampleType newAttack_ms);
    void setRelease (SampleType newRelease_ms);

    void reset();

    SampleType processChannel (int               channel,
                               int               numSamples,
                               SampleType*       signalToGate,
                               const SampleType* sidechain) final;

private:
    SampleType processSample (const int        channel,
                              const SampleType sampleToGate,
                              const SampleType sidechainValue,
                              SampleType*      gainReduction);

    void update();

    juce::dsp::ProcessSpec spec;

    SampleType                                threshold, thresholdInverse, currentRatio;
    juce::dsp::BallisticsFilter< SampleType > envelopeFilter, RMSFilter;

    SampleType thresholddB = -100, ratio = 10.0, attackTime = 1.0,
               releaseTime = 100.0;

    bool inverted = false;
};

}  // namespace bav::dsp::FX
