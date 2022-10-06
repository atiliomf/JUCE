#pragma once

namespace bav::dsp
{
template < typename SampleType >
class MidiChoppingProcessor
{
public:
    MidiChoppingProcessor();
    virtual ~MidiChoppingProcessor() = default;

    void process (juce::AudioBuffer< SampleType >& audio, MidiBuffer& midi);

private:
    void processInternal (juce::AudioBuffer< SampleType >& audio, MidiBuffer& midi,
                          int startSample, int numSamples);

    virtual void handleMidiMessage (const MidiMessage& m)                               = 0;
    virtual void renderChunk (juce::AudioBuffer< SampleType >& audio, MidiBuffer& midi) = 0;

    MidiBuffer midiStorage;
};

}  // namespace bav::dsp
