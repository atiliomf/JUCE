
#pragma once

namespace bav::midi
{
struct PitchPipeline
{
    float getFrequencyForMidi (int midiPitch, int midiChannel = -1) const;
    float getFrequencyForMidi (float midiPitch, int midiChannel = -1) const;

    float getMidiForFrequency (int midiPitch, int midiChannel = -1) const;
    float getMidiForFrequency (float midiPitch, int midiChannel = -1) const;

    void reset();

    PitchBendTracker bend;
    PitchConverter   tuning;
};

}  // namespace bav::midi
