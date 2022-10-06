#pragma once

/*
 Saturator, based on a piecewise transfer function originally created by Evan Berard.
 */

namespace bav::dsp::FX
{
template < typename SampleType >
class Saturator : AudioEffect< SampleType >
{
public:
    void setHardness (float newHardness);

private:
    void process (juce::AudioBuffer< SampleType >& audio) final;

    SampleType processSample (SampleType inputSample);

    SampleType transferFunc (SampleType amplitude);

    SampleType phaseOne (SampleType inputSample, SampleType transferFuncSample);
    SampleType phaseTwo (SampleType inputSample, SampleType transferFuncSample, SampleType phaseOneSample);

    void prepare (double samplerate, int blocksize) final;
    void bypassedBlock (int numSamples) final;

    SampleType hardness {1.};
};

}  // namespace bav::dsp::FX
