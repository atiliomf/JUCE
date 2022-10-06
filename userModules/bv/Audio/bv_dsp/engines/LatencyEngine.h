
#pragma once

namespace bav::dsp
{
template < typename SampleType >
class LatencyEngine : public Engine< SampleType >
{
public:
    using AudioBuffer = juce::AudioBuffer< SampleType >;

    int  reportLatency() const final { return internalBlocksize; }
    void changeLatency (int newInternalBlocksize);

private:
    void renderBlock (const AudioBuffer& input, AudioBuffer& output, MidiBuffer& midiMessages, bool isBypassed) final;

    void prepared (int blocksize, double samplerate) final;
    void released() final;

    virtual void renderChunk (const AudioBuffer& input, AudioBuffer& output, MidiBuffer& midiMessages, bool isBypassed) = 0;
    virtual void onPrepare (int blocksize, double samplerate);
    virtual void onRelease() { }


    int                            internalBlocksize {0};
    AudioAndMidiFIFO< SampleType > inputFIFO, outputFIFO;
    AudioBuffer                    inBuffer, outBuffer;
    MidiBuffer                     chunkMidiBuffer;
};

}  // namespace bav::dsp
