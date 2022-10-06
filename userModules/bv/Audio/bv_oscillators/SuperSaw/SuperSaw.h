#pragma once

namespace bav::dsp::osc
{
template < typename SampleType >
class SuperSaw : public Oscillator< SampleType >
{
public:
    SuperSaw();

    void resetPhase() final;
    void setFrequency (SampleType frequency, SampleType sampleRate) final;

    void setDetuneAmount (int totalPitchSpreadInCents);
    int  getPitchSpreadCents() const;

    SampleType getSample() final;

private:
    juce::OwnedArray< Saw< SampleType > > saws;

    int totalSpreadCents {0};

    SampleType lastFrequency {440.}, samplerate {44100.};
};

}  // namespace bav::dsp::osc
