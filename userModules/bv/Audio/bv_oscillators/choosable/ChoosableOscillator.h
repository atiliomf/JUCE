
#pragma once

#include "OscEngine.h"

namespace bav::dsp::osc
{
enum OscType
{
    SineOsc,
    SawOsc,
    SquareOsc,
    TriangleOsc,
    SuperSawOsc
};

template < typename SampleType >
class ChoosableOscillator
{
public:
    using AudioBuffer = juce::AudioBuffer< SampleType >;

    ChoosableOscillator();

    void    setOscType (OscType newType);
    OscType getOscType() const { return type; }

    void  setFrequency (float freqHz);
    float getFrequency() const { return freq; }

    void process (AudioBuffer& output);

    void prepare (int blocksize, double samplerate);

    /* only relevant to super saw mode */
    void setDetuneAmount (int pitchSpreadCents);

private:
    virtual void prepared (int blocksize);

    OscType type {SineOsc};
    float   freq {440.f};

    OscEngine< SampleType, Sine >     sine;
    OscEngine< SampleType, Saw >      saw;
    OscEngine< SampleType, Square >   square;
    OscEngine< SampleType, Triangle > triangle;
    OscEngine< SampleType, SuperSaw > superSaw;
};

}  // namespace bav::dsp::osc
