
namespace bav::dsp
{
/*
 Constructor.
 */
template < typename SampleType >
SynthBase< SampleType >::SynthBase()
    : lastBlocksize (0)
{
    adsrParams.attack  = 0.035f;
    adsrParams.decay   = 0.06f;
    adsrParams.sustain = 0.8f;
    adsrParams.release = 0.01f;

    quickReleaseParams.attack  = 0.01f;
    quickReleaseParams.decay   = 0.005f;
    quickReleaseParams.sustain = 1.0f;
    quickReleaseParams.release = 0.015f;

    setConcertPitchHz (440);
}


/*
 Destructor.
 */
template < typename SampleType >
SynthBase< SampleType >::~SynthBase()
{
    voices.clear();
}


/*
 Initializes the synth.
 */
template < typename SampleType >
void SynthBase< SampleType >::initialize (int initNumVoices, double initSamplerate, int initBlocksize)
{
    jassert (initNumVoices > 0 && initSamplerate > 0 && initBlocksize > 0);

    changeNumVoices (initNumVoices);

    initialized (initSamplerate, initBlocksize);

    prepare (initSamplerate, initBlocksize);
}

template < typename SampleType >
bool SynthBase< SampleType >::isInitialized() const
{
    return getNumVoices() > 0 && getSamplerate() > 0;
}


/*
 Immediately clears all notes & resets the synth's internal state.
 */
template < typename SampleType >
void SynthBase< SampleType >::reset()
{
    for (auto* voice : voices)
        voice->clearCurrentNote();

    panner.reset();

    resetTriggered();
}


/*
 Prepares the synth for a new expected maximum blocksize.
 */
template < typename SampleType >
void SynthBase< SampleType >::prepare (double samplerate, int blocksize)
{
    jassert (blocksize > 0 && samplerate > 0);
    jassert (! voices.isEmpty());

    aggregateMidiBuffer.ensureSize (static_cast< size_t > (blocksize * 2));
    aggregateMidiBuffer.clear();

    for (auto* voice : voices)
        voice->prepare (samplerate, blocksize);

    panner.prepare (voices.size(), false);

    resetRampedValues();

    sampleRate = samplerate;

    prepared (samplerate, blocksize);
}


/*
 Attempts to reduce the synth's footprint as much as possible.
 After calling this, you must call initialize() again before attempting to render any more audio with the synth.
 */
template < typename SampleType >
void SynthBase< SampleType >::releaseResources()
{
    aggregateMidiBuffer.clear();
    usableVoices.clear();
    currentNotes.clear();
    desiredNotes.clear();

    for (auto* voice : voices)
        voice->release();

    release();
}


/*=========================================================================================================
          _    _ _____ _____ ____     _____  ______ _   _ _____  ______ _____  _____ _   _  _____
     /\  | |  | |  __ \_   _/ __ \   |  __ \|  ____| \ | |  __ \|  ____|  __ \|_   _| \ | |/ ____|
    /  \ | |  | | |  | || || |  | |  | |__) | |__  |  \| | |  | | |__  | |__) | | | |  \| | |  __
   / /\ \| |  | | |  | || || |  | |  |  _  /|  __| | . ` | |  | |  __| |  _  /  | | | . ` | | |_ |
  / ____ \ |__| | |__| || || |__| |  | | \ \| |____| |\  | |__| | |____| | \ \ _| |_| |\  | |__| |
 /_/    \_\____/|_____/_____\____/   |_|  \_\______|_| \_|_____/|______|_|  \_\_____|_| \_|\_____|
 
 Functions for audio rendering
 
 ==========================================================================================================*/

/*
 Renders the synth's composite stereo output to "output". (If a mono buffer is passed, the synth will return mono output.)
 Reads events from and returns the synth's composite midi output to "midiMessages". Note that due to the midi latch, pedal pitch, and descant features, the returned midi buffer may have more or fewer events than the original passed midi buffer.
 Internally, the synth will render smaller chunks of audio in between each midi message timestamp, to keep midi events synchronous with the audio output. If you are implementing a class that derives from this class or the SynthVoiceBase class, note that the SynthVoiceBase's renderPlease() method may recieve blocksizes as small as 1 sample (but never 0 samples).
 */
template < typename SampleType >
void SynthBase< SampleType >::renderVoices (juce::MidiBuffer& midiMessages, juce::AudioBuffer< SampleType >& output)
{
    jassert (! voices.isEmpty());
    jassert (sampleRate > 0);

    output.clear();
    aggregateMidiBuffer.clear();
    midiInputStorage.clear();

    const auto numSamples = output.getNumSamples();

    midiInputStorage.addEvents (midiMessages, 0, numSamples, 0);

    for (auto* voice : voices)
        voice->newBlockComing (lastBlocksize, numSamples);

    lastBlocksize = numSamples;

    chopper.process (output, midiMessages);

    midiMessages.swapWith (aggregateMidiBuffer);

    aggregateMidiBuffer.clear();
    midiInputStorage.clear();
}


template < typename SampleType >
void SynthBase< SampleType >::MidiChopper::handleMidiMessage (const juce::MidiMessage& m)
{
    synth.midi.process (m);
}

template < typename SampleType >
void SynthBase< SampleType >::MidiChopper::renderChunk (juce::AudioBuffer< SampleType >& audio, juce::MidiBuffer&)
{
    const auto numSamples = audio.getNumSamples();

    for (auto* voice : synth.voices)
    {
        if (voice->isVoiceActive())
            voice->renderBlock (audio);
        else
            voice->bypassedBlock (numSamples);
    }
}

/*
 This should be called when a block is recieved while the processor is in bypassed mode.
 This will not output any audio, but may alter the contents of the MidiBuffer.
 */
template < typename SampleType >
void SynthBase< SampleType >::bypassedBlock (int numSamples, MidiBuffer& midiMessages)
{
    midi.process (midiMessages);

    for (auto* voice : voices)
        voice->bypassedBlock (numSamples);
}

/*
 Resets the voices' ramped gain values, and prepares them for a new blocksize.
 This should be called with/by prepare().
 */
template < typename SampleType >
void SynthBase< SampleType >::resetRampedValues()
{
    for (auto* voice : voices)
        voice->resetRampedValues();
}


template class SynthBase< float >;
template class SynthBase< double >;


}  // namespace bav::dsp
