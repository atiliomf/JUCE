
namespace bav::dsp::FX
{
PannerBase::PannerBase()
    : lastRecievedMidiPan (64), leftGain (0.5f), rightGain (0.5f)
{
}

void PannerBase::resetToCenter()
{
    leftGain            = 0.5f;
    rightGain           = 0.5f;
    lastRecievedMidiPan = 64;
}

int PannerBase::getLastMidiPan() const noexcept { return lastRecievedMidiPan; }

float PannerBase::getLeftGain() const noexcept { return leftGain; }

float PannerBase::getRightGain() const noexcept { return rightGain; }

float PannerBase::getGainMult (const int chan) const
{
    switch (chan)
    {
        case 0 : return leftGain;
        case 1 : return rightGain;
        default : return 1.0f;
    }
}

void PannerBase::getGainMults (float& left, float& right)
{
    left  = leftGain;
    right = rightGain;
}

void PannerBase::setMidiPan (int newMidiPan)
{
    jassert (newMidiPan >= 0 && newMidiPan <= 127);

    if (lastRecievedMidiPan == newMidiPan) return;

    const auto panningAngle = juce::jlimit (
        0.0f,
        90.0f,
        (90.0f * newMidiPan / 127.0f * juce::MathConstants< float >::pi)
            / 180.0f);

    leftGain  = juce::jlimit (0.0f, 1.0f, std::sin (panningAngle));
    rightGain = juce::jlimit (0.0f, 1.0f, std::cos (panningAngle));

    lastRecievedMidiPan = newMidiPan;
}

void PannerBase::setMidiPan (int newMidiPan, float& leftGainOutput, float& rightGainOutput)
{
    setMidiPan (newMidiPan);
    leftGainOutput  = leftGain;
    rightGainOutput = rightGain;
}

}  // namespace bav::dsp::FX
