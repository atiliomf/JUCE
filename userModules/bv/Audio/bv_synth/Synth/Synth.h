
#pragma once


namespace bav::dsp
{
template < typename SampleType >
class SynthBase
{
    using AudioBuffer = juce::AudioBuffer< SampleType >;
    using uint32      = juce::uint32;
    using Voice       = SynthVoiceBase< SampleType >;
    using ADSRParams  = juce::ADSR::Parameters;


public:
    SynthBase();

    virtual ~SynthBase();

    void initialize (int initNumVoices, double initSamplerate = 44100.0, int initBlocksize = 512);
    bool isInitialized() const;

    void prepare (double samplerate, int blocksize);

    void reset();
    void resetRampedValues();

    double getSamplerate() const { return sampleRate; }

    void renderVoices (MidiBuffer& midiMessages, juce::AudioBuffer< SampleType >& output);

    void releaseResources();

    void bypassedBlock (int numSamples, MidiBuffer& midiMessages);

    void processMidiEvent (const MidiMessage& m);

    void playChord (const juce::Array< int >& desiredPitches, float velocity = 1.0f, bool allowTailOffOfOld = false);

    void allNotesOff (bool allowTailOff = false, float velocity = 1.0f);

    void turnOffAllKeyupNotes (bool  allowTailOff                = false,
                               bool  includePedalPitchAndDescant = true,
                               float velocity                    = 1.0f,
                               bool  overrideSostenutoPedal      = true);

    bool isPitchActive (int midiPitch, bool countRingingButReleased = false, bool countKeyUpNotes = false) const;
    void reportActiveNotes (juce::Array< int >& outputArray, bool includePlayingButReleased = false, bool includeKeyUpNotes = true) const;

    int  getNumActiveVoices() const;
    int  getNumVoices() const noexcept { return voices.size(); }
    void changeNumVoices (int newNumVoices);

    void setNoteStealingEnabled (bool shouldSteal) noexcept { shouldStealNotes = shouldSteal; }
    void updateMidiVelocitySensitivity (int newSensitivity);
    void updatePitchbendRange (int rangeUp, int rangeDown);
    void updatePitchbendRange (int rangeSemitones);
    void setAftertouchGainOnOff (bool shouldBeOn) { aftertouchGainIsOn = shouldBeOn; }

    void setPedalPitch (bool isOn) { pedal.setEnabled (isOn); }
    void setPedalPitchUpperThresh (int newThresh) { pedal.setThreshold (newThresh); }
    void setPedalPitchInterval (int newInterval) { pedal.setInterval (newInterval); }
    void setPedalPitch (bool isOn, int newThresh, int newInterval);

    void setDescant (bool isOn) { descant.setEnabled (isOn); }
    void setDescantLowerThresh (int newThresh) { descant.setThreshold (newThresh); }
    void setDescantInterval (int newInterval) { descant.setInterval (newInterval); }
    void setDescant (bool isOn, int newThresh, int newInterval);

    /* If you are not using MTS-ESP, you can call this method to alter the master tuning of your synth. If you are using MTS-ESP, calling this method does nothing. */
    void setConcertPitchHz (int newConcertPitchhz);

    void updateStereoWidth (int newWidth) { panner.updateStereoWidth (newWidth); }
    void updateLowestPannedNote (int newPitchThresh) { panner.setLowestNote (newPitchThresh); }

    void setMidiLatch (bool shouldBeOn, const bool allowTailOff = false);
    bool isLatched() const noexcept { return latchIsOn; }

    void updateADSRsettings (float attack, float decay, float sustain, float release);
    void updateQuickReleaseMs (int newMs);

    bool isSustainPedalDown() const noexcept { return midi.isSustainPedalDown(); }
    bool isSostenutoPedalDown() const noexcept { return midi.isSostenutoPedalDown(); }
    bool isSoftPedalDown() const noexcept { return midi.isSoftPedalDown(); }
    bool isAftertouchGainOn() const noexcept { return aftertouchGainIsOn; }

    void setPlayingButReleasedMultiplier (float newGain) { playingButReleasedMultiplier = newGain; }
    void setSoftPedalMultiplier (float newGain) { softPedalMultiplier = newGain; }

    int getLastBlocksize() const noexcept { return lastBlocksize; }

    bool         isConnectedToMtsEsp() const { return pitch.tuning.isConnectedToMtsEsp(); }
    juce::String getScaleName() const { return pitch.tuning.getScaleName(); }

    auto getLastMovedControllerInfo() const { return midi.getLastMovedCCinfo(); }

    void setPitchGlideTime (double glideTimeSeconds);
    void togglePitchGlide (bool shouldGlide);

    const midi::PitchPipeline* getPitchAdjuster() { return &pitch; }

protected:
    friend class SynthVoiceBase< SampleType >;

    // if overridden, called in the subclass when the top-level call to initialize() is made.
    virtual void initialized (double initSamplerate, int initBlocksize) { juce::ignoreUnused (initSamplerate, initBlocksize); }

    // if overridden, called in the subclass when the top-level call to prepare() is made.
    virtual void prepared (double samplerate, int blocksize) { juce::ignoreUnused (samplerate, blocksize); }

    // if overridden, called in the subclass when the top-level call to reset() is made.
    virtual void resetTriggered() { }

    // if overridden, called in the subclass when the top-level call to releaseResources() is made.
    virtual void release() { }

    // this method should return an instance of your synth's voice subclass
    virtual Voice* createVoice() = 0;

private:
    void addNumVoices (int voicesToAdd);
    void removeNumVoices (int voicesToRemove);
    void numVoicesChanged();

    void noteOn (int midiPitch, float velocity, bool isKeyboard = true, int midiChannel = -1);
    void noteOff (int midiNoteNumber, float velocity = 1.0f, bool allowTailOff = false, bool isKeyboard = true);
    void startVoice (Voice* voice, int midiPitch, float velocity, bool isKeyboard, int midiChannel = -1);
    void stopVoice (Voice* voice, float velocity, bool allowTailOff);
    void turnOnList (const juce::Array< int >& toTurnOn, float velocity = 1.0f, bool partOfChord = false);
    void turnOffList (const juce::Array< int >& toTurnOff, float velocity = 1.0f, bool allowTailOff = false, bool partOfChord = false);

    void pitchCollectionChanged();

    void updateChannelPressure (int newIncomingAftertouch);

    Voice* findFreeVoice (bool stealIfNoneAvailable = true);
    Voice* findVoiceToSteal();

    Voice* getVoicePlayingNote (int midiPitch) const;

    /*==============================================================================================================
     ===============================================================================================================*/

    juce::OwnedArray< Voice > voices;

    bool latchIsOn {false};

    juce::Array< int > currentNotes;
    juce::Array< int > desiredNotes;

    ADSRParams adsrParams;
    ADSRParams quickReleaseParams;

    double sampleRate {0.0};

    bool shouldStealNotes {true};

    midi::VelocityHelper velocityConverter;
    midi::PitchPipeline  pitch;

    bool aftertouchGainIsOn {true};

    float playingButReleasedMultiplier;

    float softPedalMultiplier;  // the multiplier by which each voice's output will be multiplied when the soft pedal is down

    juce::Array< Voice* > usableVoices;  // this array is used to sort the voices when a 'steal' is requested

    int lastBlocksize;

    MidiBuffer aggregateMidiBuffer;  // this midi buffer will be used to collect the harmonizer's aggregate MIDI output
    MidiBuffer midiInputStorage;     // each block of midi that comes in is stored in here so we can refer to it later

    //--------------------------------------------------

    class MidiChopper : public MidiChoppingProcessor< SampleType >
    {
    public:
        MidiChopper (SynthBase& s) : synth (s) { }

    private:
        void handleMidiMessage (const MidiMessage& m) final;
        void renderChunk (juce::AudioBuffer< SampleType >& audio, juce::MidiBuffer&) final;

        SynthBase& synth;
    };

    MidiChopper chopper {*this};

    //--------------------------------------------------

    class MidiManager : public midi::MidiProcessor
    {
    public:
        MidiManager (SynthBase& s) : synth (s) { }

    private:
        void handleNoteOn (int midiPitch, float velocity) final;
        void handleNoteOff (int midiPitch, float velocity) final;
        void handlePitchwheel (int wheelValue) final;
        void handleAftertouch (int noteNumber, int aftertouchValue) final;
        void handleChannelPressure (int channelPressureValue) final;
        void handleSustainPedal (int controllerValue) final;
        void handleSostenutoPedal (int controllerValue) final;
        void handleSoftPedal (int controllerValue) final;
        void handleAllSoundOff() final { synth.allNotesOff (false); }

        SynthBase& synth;
    };

    MidiManager midi {*this};

    //--------------------------------------------------

    class PanningManager
    {
        using Array = juce::Array< int >;

    public:
        PanningManager (SynthBase& b) : synth (b) { }

        void prepare (int numVoices, bool clearArrays = true);
        void reset();

        void updateStereoWidth (int newWidth);

        void setLowestNote (int newLowestNote);
        int  getLowestNote() const { return lowestPannedNote; }

        int  getNextPanVal();
        void panValTurnedOff (int panVal);

    private:
        void updatePanValueLookupTables (int newWidth);
        void mapArrayIndexes();
        int  getClosestNewPanValFromOld (int oldPan);
        int  findClosestValueInNewArray (int targetValue, Array& newArray);

        SynthBase& synth;

        int stereoWidth {100};
        int lowestPannedNote {0};

        Array arrayIndexesMapped;
        Array possiblePanVals, panValsInAssigningOrder, unsentPanVals;
        Array newPanVals, newUnsentVals;
        Array distances;
    };

    PanningManager panner {*this};

    //--------------------------------------------------

    class AutomatedHarmonyVoice
    {
    public:
        AutomatedHarmonyVoice (SynthBase& synthToUse, bool shiftUp);

        void apply();

        void setEnabled (bool shouldBeEnabled);
        void setThreshold (int newThresh);
        void setInterval (int newInterval);

        void turnNoteOffIfOn();
        void setNoteToOff() { lastPitch = -1; }

        bool isAutomatedPitch (int midiNote) { return isOn && lastPitch == midiNote; }

        // call this function when processing an automated note-off and the voice's keyboard key is still being held
        void autoNoteOffKeyboardKeyHeld (int midiNote);

        Voice* getVoice();

    private:
        const bool shiftingUp;

        bool isOn {false};
        int  lastPitch {-1};
        int  thresh {0};
        int  interval {12};

        SynthBase& synth;
    };

    AutomatedHarmonyVoice pedal {*this, false};
    AutomatedHarmonyVoice descant {*this, true};
};


}  // namespace bav::dsp
