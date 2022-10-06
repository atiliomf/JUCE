
#pragma once

namespace bav::midi
{
class MidiProcessor
{
public:
    using Metadata = juce::MidiMessageMetadata;

    void reset();

    void process (const MidiBuffer& buffer);
    void process (const Metadata& meta);
    void process (const MidiMessage& m);

    int getLastPitchwheelValue() const { return lastPitchwheelValue; }
    int getLastMidiChannel() const { return lastMidiChannel; }
    int getLastMidiTimestamp() const { return lastMidiTimestamp; }

    bool isSustainPedalDown() const { return sustainPedalDown; }
    bool isSostenutoPedalDown() const { return sostenutoPedalDown; }
    bool isSoftPedalDown() const { return softPedalDown; }

    struct LastMovedControllerInfo
    {
        int controllerNumber, controllerValue;
    };

    LastMovedControllerInfo getLastMovedCCinfo() const;

private:
    void processControllerMessage (int controllerNumber, int controllerValue);
    void processPitchWheel (int wheelValue);
    void processSustainPedal (int controllerValue);
    void processSostenutoPedal (int controllerValue);
    void processSoftPedal (int controllerValue);

    virtual void handleNoteOn (int midiPitch, float velocity);
    virtual void handleNoteOff (int midiPitch, float velocity);
    virtual void handlePitchwheel (int wheelValue);
    virtual void handleAftertouch (int noteNumber, int aftertouchValue);
    virtual void handleChannelPressure (int channelPressureValue);
    virtual void handleSustainPedal (int controllerValue);
    virtual void handleSostenutoPedal (int controllerValue);
    virtual void handleSoftPedal (int controllerValue);
    virtual void handleController (int controllerNumber, int controllerValue);
    virtual void handleAllSoundOff() { }

    int lastMidiChannel {1};
    int lastMidiTimestamp {0};

    int lastPitchwheelValue {64};

    int lastMovedController {0};
    int lastControllerValue {0};

    bool sustainPedalDown {false};
    bool sostenutoPedalDown {false};
    bool softPedalDown {false};
};

}  // namespace bav::midi
