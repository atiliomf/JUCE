
namespace bav::dsp
{
template < typename SampleType >
void SynthBase< SampleType >::MidiManager::handleNoteOn (int midiPitch, float velocity)
{
    synth.noteOn (midiPitch, velocity, true, getLastMidiChannel());
}

template < typename SampleType >
void SynthBase< SampleType >::MidiManager::handleNoteOff (int midiPitch, float velocity)
{
    synth.noteOff (midiPitch, velocity, true, getLastMidiChannel());
}

template < typename SampleType >
void SynthBase< SampleType >::MidiManager::handleAftertouch (int noteNumber, int aftertouchValue)
{
    jassert (noteNumber >= 0 && noteNumber <= 127);
    jassert (aftertouchValue >= 0 && aftertouchValue <= 127);

    synth.aggregateMidiBuffer.addEvent (MidiMessage::aftertouchChange (synth.midi.getLastMidiChannel(),
                                                                       noteNumber, aftertouchValue),
                                        synth.midi.getLastMidiTimestamp());

    for (auto* voice : synth.voices)
        if (voice->isVoiceActive() && voice->getCurrentlyPlayingNote() == noteNumber)
            voice->aftertouchChanged (aftertouchValue);
}

template < typename SampleType >
void SynthBase< SampleType >::MidiManager::handleChannelPressure (int channelPressureValue)
{
    jassert (channelPressureValue >= 0 && channelPressureValue <= 127);

    synth.aggregateMidiBuffer.addEvent (MidiMessage::channelPressureChange (synth.midi.getLastMidiChannel(),
                                                                            channelPressureValue),
                                        synth.midi.getLastMidiTimestamp());

    for (auto* voice : synth.voices)
        voice->aftertouchChanged (channelPressureValue);
}

template < typename SampleType >
void SynthBase< SampleType >::MidiManager::handleSustainPedal (int controllerValue)
{
    const bool isDown = (controllerValue >= 64);

    synth.aggregateMidiBuffer.addEvent (MidiMessage::controllerEvent (synth.midi.getLastMidiChannel(), 0x40, controllerValue),
                                        synth.midi.getLastMidiTimestamp());

    if (! isDown && ! synth.latchIsOn)
        synth.turnOffAllKeyupNotes (false, false, 0.0f, false);
}

template < typename SampleType >
void SynthBase< SampleType >::MidiManager::handleSostenutoPedal (int controllerValue)
{
    const bool isDown = (controllerValue >= 64);

    synth.aggregateMidiBuffer.addEvent (MidiMessage::controllerEvent (synth.midi.getLastMidiChannel(),
                                                                      0x42, controllerValue),
                                        synth.midi.getLastMidiTimestamp());

    if (isDown && ! synth.latchIsOn)
    {
        for (auto* voice : synth.voices)
            if (voice->isVoiceActive() && ! voice->isPedalPitchVoice && ! voice->isDescantVoice)
                voice->sustainingFromSostenutoPedal = true;
    }
    else
    {
        synth.turnOffAllKeyupNotes (false, false, 0.0f, true);
    }
}

template < typename SampleType >
void SynthBase< SampleType >::MidiManager::handleSoftPedal (int controllerValue)
{
    const bool isDown = controllerValue >= 64;

    synth.aggregateMidiBuffer.addEvent (MidiMessage::controllerEvent (synth.midi.getLastMidiChannel(),
                                                                      0x43, controllerValue),
                                        synth.midi.getLastMidiTimestamp());

    for (auto* voice : synth.voices)
        voice->softPedalChanged (isDown);
}

template < typename SampleType >
void SynthBase< SampleType >::MidiManager::handlePitchwheel (int wheelValue)
{
    jassert (wheelValue >= 0 && wheelValue <= 127);

    synth.aggregateMidiBuffer.addEvent (MidiMessage::pitchWheel (synth.midi.getLastMidiChannel(),
                                                                 wheelValue),
                                        synth.midi.getLastMidiTimestamp());

    synth.pitch.bend.newPitchbendRecieved (wheelValue);

    for (auto* voice : synth.voices)
        if (voice->isVoiceActive())
            voice->setTargetOutputFrequency (synth.pitch.getFrequencyForMidi (voice->getCurrentlyPlayingNote()));
}

}  // namespace bav::dsp
