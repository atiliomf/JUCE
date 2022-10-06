
#pragma once

namespace bav::dsp::FX
{
/*
    Simple limiter that allows you to sidechain the signal.
*/

template < typename SampleType >
class Limiter : public LevelReportingAudioEffect< SampleType >
{
public:
    void prepare (double samplerate, int blocksize) final;

    void setThreshold (float thresh_dB);
    void setRelease (float release_ms);

    void reset();

private:
    SampleType processChannel (int               channel,
                               int               numSamples,
                               SampleType*       signalToLimit,
                               const SampleType* sidechain) final;

    void update();

    Compressor< SampleType > firstStageCompressor, secondStageCompressor;

    juce::SmoothedValue< SampleType, juce::ValueSmoothingTypes::Linear >
        outputVolume;

    double sampleRate  = 44100.0;
    float  thresholddB = -10.0, releaseTime = 100.0;
};

}  // namespace bav::dsp::FX
