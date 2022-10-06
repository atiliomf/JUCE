
#pragma once

namespace bav::midi
{
// returns true if the given midinote is a black key
template < typename noteType >
bool isMidiNoteBlackKey (noteType midipitch);


// returns true if the two pitches are the same pitch class, regardless of octave
template < typename noteType1, typename noteType2 >
bool areNotesSamePitchClass (const noteType1 pitch1, const noteType2 pitch2);


// copies a range of events from one juce::MidiBuffer to another, optionally providing a sample offset
void copyRangeOfMidiBuffer (const MidiBuffer& readingBuffer,
                            MidiBuffer&       destBuffer,
                            const int         startSampleOfInput,
                            const int         startSampleOfOutput,
                            const int         numSamples);

// returns true if the passed int is between 0 and 127
bool isValidMidiPitch (int note);


extern float getGainMultFromMidiVelocity (float midiVelocity, float sensitivity = 1.0f);
extern float getGainMultFromMidiVelocity (int midiVelocity, float sensitivity = 1.0f);


struct VelocityHelper
{
    void setSensitivity (float newSensitivity);
    void setSensitivity (int newSensitivity);

    float getGainForVelocity (float midiVelocity);
    float getGainForVelocity (int midiVelocity);

    float getSensitivity() const { return sensitivity; }

private:
    float sensitivity {1.0f};
};


}  // namespace bav::midi
