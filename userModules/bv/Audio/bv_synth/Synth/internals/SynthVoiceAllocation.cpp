
namespace bav::dsp
{
/*
 Attempts to find the appropriate voice to start when a note-on event is recieved.
 */
template < typename SampleType >
SynthVoiceBase< SampleType >* SynthBase< SampleType >::findFreeVoice (bool stealIfNoneAvailable)
{
    jassert (! voices.isEmpty());

    /* Look into the future!  If a voice has a note off coming within the next few milliseconds, let's steal that voice... */
    constexpr int futureStealingMaxMs = 10;

    for (auto* voice : voices)
    {
        if (! voice->isVoiceActive()) continue;

        const auto note = voice->getCurrentlyPlayingNote();

        for (auto midiIterator = midiInputStorage.findNextSamplePosition (midi.getLastMidiTimestamp() + 1);
             midiIterator != midiInputStorage.findNextSamplePosition (midi.getLastMidiTimestamp() + futureStealingMaxMs + 1);
             ++midiIterator)
        {
            const auto metadata = *midiIterator;
            const auto msg      = metadata.getMessage();

            if (msg.isNoteOff() && msg.getNoteNumber() == note) return voice;
        }
    }

    for (auto* voice : voices)
        if (! voice->isVoiceActive()) return voice;

    if (stealIfNoneAvailable) return findVoiceToSteal();

    return nullptr;
}


/*
 If findFreeVoice() is called and every voice is active, this function will attempt to find the optimal voice to "steal" for the new note.
 This voice stealing algorithm protects the highest & lowest notes whose keyboard keys are still being held, and the pedal & descant voices, if they're active.
 */
template < typename SampleType >
SynthVoiceBase< SampleType >* SynthBase< SampleType >::findVoiceToSteal()
{
    // These are the voices we want to protect
    Voice* low = nullptr;  // Lowest sounding note, might be sustained, but NOT in release phase
    Voice* top = nullptr;  // Highest sounding note, might be sustained, but NOT in release phase

    // protect these, only use if necessary. These will be nullptrs if pedal / descant is currently off
    auto* descantVoice = descant.getVoice();
    auto* pedalVoice   = pedal.getVoice();

    usableVoices.clearQuick();  // this is a list of voices we can steal, sorted by how long they've been on

    for (auto* voice : voices)
    {
        if (voice == descantVoice || voice == pedalVoice) continue;

        usableVoices.add (voice);

        // NB: Using a functor rather than a lambda here due to scare-stories about compilers generating code containing heap allocations...
        struct Sorter
        {
            bool operator() (const Voice* a, const Voice* b) const noexcept { return a->wasStartedBefore (*b); }
        };

        std::sort (usableVoices.begin(), usableVoices.end(), Sorter());

        if (voice->isVoiceActive() && ! voice->isPlayingButReleased())
        {
            auto note = voice->getCurrentlyPlayingNote();

            if (low == nullptr || note < low->getCurrentlyPlayingNote()) low = voice;

            if (top == nullptr || note > top->getCurrentlyPlayingNote()) top = voice;
        }
    }

    if (top == low)  // Eliminate pathological cases (ie: only 1 note playing): we always give precedence to the lowest note(s)
        top = nullptr;

    for (auto* voice : usableVoices)
        if (voice != low && voice != top && ! voice->isKeyDown()) return voice;

    for (auto* voice : usableVoices)
        if (voice != low && voice != top) return voice;

    // only protected top & bottom voices are left now - time to use the pedal pitch & descant voices...

    if (descantVoice != nullptr)  // save bass
        return descantVoice;

    if (pedalVoice != nullptr) return pedalVoice;

    // return final top & bottom notes held with keyboard keys

    if (top != nullptr)  // save bass
        return top;

    return low;
}


/*
 Call this function to change the number of voices the synth is managing.
 Note that this function will allocate!
 */
template < typename SampleType >
void SynthBase< SampleType >::changeNumVoices (int newNumVoices)
{
    const auto currentVoices = voices.size();

    if (currentVoices == newNumVoices) return;

    if (newNumVoices > currentVoices)
        addNumVoices (newNumVoices - currentVoices);
    else
        removeNumVoices (currentVoices - newNumVoices);

    jassert (voices.size() == newNumVoices);
}


/*
 Adds a specified number of voices to the synth.
 This is virtual because your subclass can override this to add instances of your subclass of SynthVoiceBase. If you provide a custom implementation, your function MUST call numVoicesChanged() after it has finished adding voices!
 */
template < typename SampleType >
void SynthBase< SampleType >::addNumVoices (int voicesToAdd)
{
    if (voicesToAdd == 0) return;

    for (int i = 0; i < voicesToAdd; ++i)
        voices.add (createVoice());

    jassert (voices.size() >= voicesToAdd);

    numVoicesChanged();
}


/*
 Removes a specified number of voices from the synth. This function attempts to remove inactive voices first.
 */
template < typename SampleType >
void SynthBase< SampleType >::removeNumVoices (int voicesToRemove)
{
    if (voicesToRemove == 0) return;

#if JUCE_DEBUG
    const auto shouldBeLeft = voices.size() - voicesToRemove;
#endif

    int voicesRemoved = 0;

    while (voicesRemoved < voicesToRemove && ! voices.isEmpty())
    {
        if (voices.isEmpty()) break;

        Voice* removing = findFreeVoice (true);

        if (removing == nullptr) removing = voices[0];

        if (removing->isVoiceActive())
        {
            panner.panValTurnedOff (removing->getCurrentMidiPan());
            aggregateMidiBuffer.addEvent (MidiMessage::noteOff (removing->getMidiChannel(), removing->getCurrentlyPlayingNote(), 1.0f),
                                          midi.getLastMidiTimestamp());
        }

        voices.removeObject (removing, true);

        ++voicesRemoved;
    }

    jassert (voices.isEmpty() || voices.size() == shouldBeLeft);

    numVoicesChanged();
}


/*
 This function should be called any time the synth's number of voices is changed.
 */
template < typename SampleType >
void SynthBase< SampleType >::numVoicesChanged()
{
    const auto newMaxNumVoices = voices.size();

    panner.prepare (newMaxNumVoices, false);
    usableVoices.ensureStorageAllocated (newMaxNumVoices);
    currentNotes.ensureStorageAllocated (newMaxNumVoices);
    desiredNotes.ensureStorageAllocated (newMaxNumVoices);
}


/*
 Returns a pointer to the voice playing a certain note. This will return nullptr if the note is not currently active.
 */
template < typename SampleType >
SynthVoiceBase< SampleType >* SynthBase< SampleType >::getVoicePlayingNote (int midiPitch) const
{
    auto it = std::find_if (voices.begin(),
                            voices.end(),
                            [&midiPitch] (SynthVoiceBase< SampleType >* voice)
                            { return voice->isVoiceActive() && voice->getCurrentlyPlayingNote() == midiPitch; });

    return it != voices.end() ? *it : nullptr;
}


/*
 Returns the number of currently active voices.
 Voices in any state are counted - including playing-but-released, sustaining from sostenuto pedal, etc.
 */
template < typename SampleType >
int SynthBase< SampleType >::getNumActiveVoices() const
{
    return static_cast< int > (
        std::count_if (voices.begin(), voices.end(), [] (SynthVoiceBase< SampleType >* voice)
                       { return voice->isVoiceActive(); }));
}

}  // namespace bav::dsp
