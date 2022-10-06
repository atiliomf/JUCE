
namespace bav::midi
{
template < typename noteType >
bool isMidiNoteBlackKey (noteType midipitch)
{
    jassert (midipitch >= noteType (0) && midipitch <= noteType (127));

    switch (midipitch % 12)
    {
        case 1 : return true;
        case 3 : return true;
        case 6 : return true;
        case 8 : return true;
        case 10 : return true;
        default : return false;
    }
}

template < typename noteType1, typename noteType2 >
bool areNotesSamePitchClass (const noteType1 pitch1, const noteType2 pitch2)
{
    jassert (pitch1 >= noteType1 (0) && pitch2 >= noteType2 (0)
             && pitch1 <= noteType1 (127) && pitch2 <= noteType2 (127));
    return (pitch1 % 12 == pitch2 % 12);
}


void copyRangeOfMidiBuffer (const MidiBuffer& readingBuffer,
                            MidiBuffer&       destBuffer,
                            const int         startSampleOfInput,
                            const int         startSampleOfOutput,
                            const int         numSamples)
{
    if (numSamples == 0) return;

    destBuffer.clear (startSampleOfOutput, numSamples);
    destBuffer.addEvents (readingBuffer,
                          startSampleOfInput,
                          numSamples,
                          startSampleOfOutput - startSampleOfInput);
}


bool isValidMidiPitch (int note)
{
    return note >= 0 && note <= 127;
}


float getGainMultFromMidiVelocity (float midiVelocity, float sensitivity)
{
    jassert (midiVelocity >= 0.0f && midiVelocity <= 1.0f && sensitivity >= 0.0f && sensitivity <= 1.0f);
    return (1.0f - midiVelocity) * (1.0f - sensitivity) + midiVelocity;
}

float getGainMultFromMidiVelocity (int midiVelocity, float sensitivity)
{
    jassert (midiVelocity >= 0 && midiVelocity <= 127);
    constexpr auto inv127 = 1.0f / 127.0f;
    return getGainMultFromMidiVelocity (static_cast< float > (midiVelocity) * inv127,
                                        sensitivity);
}


void VelocityHelper::setSensitivity (float newSensitivity)
{
    jassert (newSensitivity >= 0.0f && newSensitivity <= 1.0f);
    sensitivity = newSensitivity;
}

void VelocityHelper::setSensitivity (int newSensitivity)
{
    setSensitivity (static_cast< float > (newSensitivity) * 0.01f);
}

float VelocityHelper::getGainForVelocity (float midiVelocity)
{
    return getGainMultFromMidiVelocity (midiVelocity, sensitivity);
}

float VelocityHelper::getGainForVelocity (int midiVelocity)
{
    return getGainMultFromMidiVelocity (midiVelocity, sensitivity);
}


MidiFIFO::MidiFIFO (int maxNumMessages) { setSize (maxNumMessages); }

void MidiFIFO::setSize (int maxNumMessages)
{
    const auto messages = size_t (maxNumMessages);
    base.ensureSize (messages);
    copying.ensureSize (messages);
}


void MidiFIFO::clear()
{
    base.clear();
    copying.clear();
    numStoredSamples = 0;
}


int MidiFIFO::numStoredEvents() const { return base.getNumEvents(); }


void MidiFIFO::pushEvents (const juce::MidiBuffer& source, const int numSamples)
{
    base.addEvents (source, 0, numSamples, numStoredSamples);
    numStoredSamples += numSamples;
}


void MidiFIFO::popEvents (juce::MidiBuffer& output, const int numSamples)
{
    output.clear();
    output.addEvents (base, 0, numSamples, 0);

    // Move all the remaining events forward by the number of samples removed
    copying.clear();
    copying.addEvents (base, numSamples, numStoredSamples, -numSamples);

    base.swapWith (copying);
    numStoredSamples = std::max (0, numStoredSamples - numSamples);
}


float PitchPipeline::getFrequencyForMidi (int midiPitch, int midiChannel) const
{
    return getFrequencyForMidi (static_cast< float > (midiPitch), midiChannel);
}

float PitchPipeline::getFrequencyForMidi (float midiPitch, int midiChannel) const
{
    return tuning.midiToFrequency (bend.getAdjustedMidiPitch (midiPitch),
                                   midiChannel);
}

float PitchPipeline::getMidiForFrequency (int midiPitch, int midiChannel) const
{
    return getMidiForFrequency (static_cast< float > (midiPitch), midiChannel);
}

float PitchPipeline::getMidiForFrequency (float midiPitch, int midiChannel) const
{
    return bend.getAdjustedMidiPitch (tuning.frequencyToMidi (math::midiToFreq (midiPitch),
                                                              midiChannel));
}

void PitchPipeline::reset()
{
    bend.newPitchbendRecieved (64);
    tuning.setConcertPitchHz (440.0f);
}


void PitchBendTracker::setRange (int newStUp, int newStDown) noexcept
{
    rangeUp   = newStUp;
    rangeDown = newStDown;
}

int PitchBendTracker::getRangeUp() const noexcept { return rangeUp; }
int PitchBendTracker::getRangeDown() const noexcept { return rangeDown; }

int PitchBendTracker::getLastRecievedPitchbend() const noexcept
{
    return lastRecievedPitchbend;
}

void PitchBendTracker::newPitchbendRecieved (const int newPitchbend)
{
    jassert (newPitchbend >= 0 && newPitchbend <= 127);
    lastRecievedPitchbend = newPitchbend;
}

}  // namespace bav::midi
