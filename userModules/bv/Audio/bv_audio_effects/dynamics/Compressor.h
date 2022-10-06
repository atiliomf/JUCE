
#pragma once

namespace bav::dsp::FX
{
/*
    Simple compressor that allows you to sidechain the signal.
*/

template < typename SampleType >
class Compressor : public LevelReportingAudioEffect< SampleType >
{
public:
    Compressor();

    void prepare (double samplerate, int blocksize) final;

    /* returns the output compressed sample */
    SampleType processSample (int         channel,
                              SampleType  inputSample,
                              SampleType  sidechainSample,
                              SampleType* gainReduction);

    void setThreshold (float newThresh_dB);
    void setRatio (float newRatio);
    void setAttack (float attackMs);
    void setRelease (float releaseMs);

    void reset();

private:
    SampleType processChannel (int               channel,
                               int               numSamples,
                               SampleType*       signalToCompress,
                               const SampleType* sidechain) final;

    void update();

    SampleType                                threshold, thresholdInverse, ratioInverse;
    juce::dsp::BallisticsFilter< SampleType > envelopeFilter;

    juce::dsp::ProcessSpec spec;

    double     sampleRate  = 44100.0;
    SampleType thresholddB = 0.0, ratio = 1.0, attackTime = 1.0, releaseTime = 100.0;
};

}  // namespace bav::dsp::FX
