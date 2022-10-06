
namespace bav::midi
{
#if BV_USE_MTS_ESP
void PitchConverter::Deleter::operator() (MTSClient* c)
{
    MTS_DeregisterClient (c);
}
#endif

float PitchConverter::midiToFrequency (int midiNote, int midiChannel) const
{
#if BV_USE_MTS_ESP
    return static_cast< float > (MTS_NoteToFrequency (client.get(), char (midiNote), char (midiChannel)));
#else
    juce::ignoreUnused (midiChannel);
    return static_cast< float > (concertPitchHz
                                 * std::pow (2.0f,
                                             ((static_cast< float > (midiNote) - 69.0f) / 12.0f)));
#endif
}

float PitchConverter::midiToFrequency (float midiNote, int midiChannel) const
{
#if BV_USE_MTS_ESP
    return static_cast< float > (MTS_NoteToFrequency (client.get(), char (midiNote), char (midiChannel)));
#else
    juce::ignoreUnused (midiChannel);
    return static_cast< float > (concertPitchHz
                                 * std::pow (2.0f,
                                             ((midiNote - 69.0f) / 12.0f)));
#endif
}

int PitchConverter::frequencyToMidi (float frequency, int midiChannel) const
{
#if BV_USE_MTS_ESP
    return juce::roundToInt (MTS_FrequencyToNote (client.get(), static_cast< double > (frequency), char (midiChannel)));
#else
    juce::ignoreUnused (midiChannel);
    return juce::roundToInt (12.0f
                                 * log2 (frequency / concertPitchHz)
                             + 69.0f);
#endif
}

bool PitchConverter::shouldFilterNote (int midiPitch, int midiChannel) const
{
#if BV_USE_MTS_ESP
    return MTS_ShouldFilterNote (client.get(), char (midiPitch), char (midiChannel));
#else
    juce::ignoreUnused (midiPitch, midiChannel);
    return false;
#endif
}

bool PitchConverter::isConnectedToMtsEsp() const
{
#if BV_USE_MTS_ESP
    return MTS_HasMaster (client.get());
#else
    return false;
#endif
}

String PitchConverter::getScaleName() const
{
#if BV_USE_MTS_ESP
    return {MTS_GetScaleName (client.get())};
#else
    return {};
#endif
}

bool PitchConverter::setConcertPitchHz (float newConcertPitchHz)
{
#if BV_USE_MTS_ESP
    juce::ignoreUnused (newConcertPitchHz);
    return false;
#else
    if (concertPitchHz == newConcertPitchHz)
        return false;

    concertPitchHz = newConcertPitchHz;
    return true;
#endif
}


}  // namespace bav::midi
