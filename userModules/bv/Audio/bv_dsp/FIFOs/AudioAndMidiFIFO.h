#pragma once

#include <bv_midi/bv_midi.h>

namespace bav::dsp
{
template < typename SampleType >
class AudioAndMidiFIFO
{
public:
    using AudioBuffer = juce::AudioBuffer< SampleType >;

    AudioAndMidiFIFO (int channels = 2, int samples = 1024);

    void setSize (int numChannels, int numSamples);

    void push (const AudioBuffer& audioIn, const MidiBuffer& midiIn);

    void pop (AudioBuffer& audioOut, MidiBuffer& midiOut);

    int numStoredSamples() const;

private:
    midi::MidiFIFO               midi;
    MultiAudioFIFO< SampleType > audio;
};

}  // namespace bav::dsp
