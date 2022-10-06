#pragma once

/*
    Simple de-esser that works by sending the input signal through an inverted noise gate sidechained by a hi-passed version of the original signal.
*/

namespace bav::dsp::FX
{
template < typename SampleType >
class DeEsser : public LevelReportingAudioEffect< SampleType >
{
public:
    DeEsser();

    void prepare (double samplerate, int blocksize) final;

    void reset();

    void setThresh (float newThresh_dB);
    void setDeEssAmount (int newAmount);

private:
    SampleType processChannel (int               channel,
                               int               numSamples,
                               SampleType*       signalToDeEss,
                               const SampleType* sidechain) final;

    dsp::filters::MultiFilter< SampleType, 2 > filter;

    NoiseGate< SampleType > gate;

    static constexpr double hiPassFreq = 7600.;
    static constexpr int    attackMs   = 20;
    static constexpr int    releaseMs  = 30;
};

}  // namespace bav::dsp::FX
