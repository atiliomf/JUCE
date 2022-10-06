
namespace bav::midi
{
void MidiProcessor::reset()
{
    lastMidiTimestamp   = 0;
    lastPitchwheelValue = 64;
    sustainPedalDown    = false;
    sostenutoPedalDown  = false;
    softPedalDown       = false;
}

void MidiProcessor::process (const MidiBuffer& buffer)
{
    std::for_each (buffer.begin(), buffer.end(),
                   [this] (const juce::MidiMessageMetadata& m)
                   { process (m); });
}

void MidiProcessor::process (const Metadata& meta)
{
    process (meta.getMessage());
}

void MidiProcessor::process (const MidiMessage& m)
{
    lastMidiChannel   = m.getChannel();
    lastMidiTimestamp = static_cast< int > (m.getTimeStamp());

    if (m.isNoteOn())
        handleNoteOn (m.getNoteNumber(), m.getFloatVelocity());
    else if (m.isNoteOff())
        handleNoteOff (m.getNoteNumber(), m.getFloatVelocity());
    else if (m.isPitchWheel())
        processPitchWheel (juce::jmap (m.getPitchWheelValue(), 0, 16383, 0, 127));
    else if (m.isAftertouch())
        handleAftertouch (m.getNoteNumber(), m.getAfterTouchValue());
    else if (m.isChannelPressure())
        handleChannelPressure (m.getChannelPressureValue());
    else if (m.isController())
        processControllerMessage (m.getControllerNumber(), m.getControllerValue());
    else if (m.isAllNotesOff() || m.isAllSoundOff())
        handleAllSoundOff();
}

void MidiProcessor::processPitchWheel (int wheelValue)
{
    lastPitchwheelValue = wheelValue;
    handlePitchwheel (wheelValue);
}

void MidiProcessor::processControllerMessage (int controllerNumber, int controllerValue)
{
    lastMovedController = controllerNumber;
    lastControllerValue = controllerValue;

    switch (controllerNumber)
    {
        case 0x40 : processSustainPedal (controllerValue); return;
        case 0x42 : processSostenutoPedal (controllerValue); return;
        case 0x43 : processSoftPedal (controllerValue); return;
        default : handleController (controllerNumber, controllerValue);
    }
}

void MidiProcessor::processSustainPedal (int controllerValue)
{
    sustainPedalDown = controllerValue >= 64;
    handleSustainPedal (controllerValue);
}

void MidiProcessor::processSostenutoPedal (int controllerValue)
{
    sostenutoPedalDown = controllerValue >= 64;
    handleSostenutoPedal (controllerValue);
}

void MidiProcessor::processSoftPedal (int controllerValue)
{
    softPedalDown = controllerValue >= 64;
    handleSoftPedal (controllerValue);
}

MidiProcessor::LastMovedControllerInfo MidiProcessor::getLastMovedCCinfo() const
{
    LastMovedControllerInfo info;
    info.controllerNumber = lastMovedController;
    info.controllerValue  = lastControllerValue;
    return info;
}

void MidiProcessor::handleNoteOn (int, float) { }
void MidiProcessor::handleNoteOff (int, float) { }
void MidiProcessor::handlePitchwheel (int) { }
void MidiProcessor::handleAftertouch (int, int) { }
void MidiProcessor::handleChannelPressure (int) { }
void MidiProcessor::handleSustainPedal (int) { }
void MidiProcessor::handleSostenutoPedal (int) { }
void MidiProcessor::handleSoftPedal (int) { }
void MidiProcessor::handleController (int, int) { }

}  // namespace bav::midi
