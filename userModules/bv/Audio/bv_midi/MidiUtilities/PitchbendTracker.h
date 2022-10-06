
#pragma once

namespace bav::midi
{
// helper class for outputting midifloat values based on input midiPitch, pitchbend, and pitchbend range controls
class PitchBendTracker
{
public:
    void setRange (int newStUp, int newStDown) noexcept;

    int getRangeUp() const noexcept;
    int getRangeDown() const noexcept;

    int getLastRecievedPitchbend() const noexcept;

    void newPitchbendRecieved (const int newPitchbend);

    template < typename NoteType >
    float getAdjustedMidiPitch (NoteType recievedMidiPitch) const
    {
        return getMidifloat (recievedMidiPitch, lastRecievedPitchbend);
    }

private:
    int rangeUp {2};
    int rangeDown {2};
    int lastRecievedPitchbend {64};

    template < typename NoteType >
    float getMidifloat (NoteType midiPitch, int pitchbend) const
    {
        if (pitchbend == 64) return static_cast< float > (midiPitch);

        if (pitchbend > 64)
            return static_cast< float > (((float (rangeUp) * (float (pitchbend) - 65.0f)) / 62.0f)
                                         + float (midiPitch));

        return static_cast< float > ((((1.0f - float (rangeDown)) * float (pitchbend)) / 63.0f) + float (midiPitch)
                                     - float (rangeDown));
    }
};

}  // namespace bav::midi
