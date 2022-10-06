
#pragma once

#include <bv_oscillators/bv_oscillators.h>

namespace bav::dsp
{
template < typename SampleType, template < typename NumericType > class OscType >
class BasicSynthVoice : public SynthVoiceBase< SampleType >
{
public:
    using SynthVoiceBase< SampleType >::SynthVoiceBase;

    void renderPlease (juce::AudioBuffer< SampleType >& output, float desiredFrequency, double currentSamplerate) final
    {
        osc.setFrequency (SampleType (desiredFrequency), SampleType (currentSamplerate));
        osc.getSamples (output.getWritePointer (0), output.getNumSamples());
    }

    void released() final
    {
        osc.resetPhase();
    }

    void noteCleared() final
    {
        osc.resetPhase();
    }

private:
    OscType< SampleType > osc;
};

template < typename SampleType, template < typename NumericType > class OscType >
struct BasicSynth : public SynthBase< SampleType >
{
    SynthVoiceBase< SampleType >* createVoice() final
    {
        return new BasicSynthVoice< SampleType, OscType > (this);
    }
};

template < typename SampleType >
using SineSynth = BasicSynth< SampleType, osc::Sine >;

}  // namespace bav::dsp
