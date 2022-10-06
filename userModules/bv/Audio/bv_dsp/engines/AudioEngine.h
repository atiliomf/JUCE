
#pragma once

namespace bav::dsp
{
template < typename SampleType >
class Engine
{
public:
    using AudioBuffer = juce::AudioBuffer< SampleType >;

    Engine()          = default;
    virtual ~Engine() = default;

    virtual int reportLatency() const { return 0; }

    void process (AudioBuffer& inplaceInAndOut,
                  MidiBuffer&  midiMessages,
                  bool         isBypassed = false);

    void process (const AudioBuffer& input,
                  AudioBuffer&       output,
                  MidiBuffer&        midiMessages,
                  bool               isBypassed = false);

    // call these two functions to use the engine without MIDI
    // (the engine's internal renderBlock() will recieve an empty midiBuffer)
    void process (AudioBuffer& inplaceInAndOut,
                  bool         isBypassed = false);

    void process (const AudioBuffer& input,
                  AudioBuffer&       output,
                  bool               isBypassed = false);

    bool isInitialized() const { return hasBeenInitialized; }

    void prepare (double samplerate, int blocksize);

    void releaseResources();

    double getSamplerate() const { return sampleRate; }

private:
    void processInternal (const AudioBuffer& input, AudioBuffer& output, MidiBuffer& midiMessages, bool isBypassed);

    virtual void renderBlock (const AudioBuffer& input, AudioBuffer& output, MidiBuffer& midiMessages, bool isBypassed) = 0;
    virtual void prepared (int blocksize, double samplerate);
    virtual void released() { }

    MidiBuffer dummyMidiBuffer;
    bool       wasBypassedLastCallback {true};
    bool       hasBeenInitialized {false};
    double     sampleRate {0.};
};

}  // namespace bav::dsp
