#pragma once


namespace bav::dsp::FX
{
/*
    handy little class that calculates right and left gain values that should be applied to a mono signal in order to place at a desired stereo location.
 
    The stereo location is set using midi pan from 0 to 127 -- 0 being hard left, 64 being center, and 127 being hard right.
 
    Note that this class does no value smoothing, and simply outputs the next target value for the left and right channels each time the panning is changed. You may want to feed this class's output to a juce::SmoothedValue, or employ some other smoothing mechanism.
*/

class PannerBase
{
public:
    PannerBase();
    virtual ~PannerBase() = default;

    void resetToCenter();

    float getLeftGain() const noexcept;
    float getRightGain() const noexcept;
    float getGainMult (const int chan) const;
    void  getGainMults (float& left, float& right);

    void setMidiPan (int newMidiPan);
    void setMidiPan (int newMidiPan, float& leftGainOutput, float& rightGainOutput);
    int  getLastMidiPan() const noexcept;

private:
    int lastRecievedMidiPan;

    float leftGain;
    float rightGain;
};


}  // namespace bav::dsp::FX
