
namespace bav
{
DeviceRotationListener::DeviceRotationListener (std::function< void() > callbackToUse)
    : callback (std::move (callbackToUse))
{
#if JUCE_IOS || JUCE_ANDROID
    Timer::startTimerHz (60);
#endif
}

DeviceRotationListener::~DeviceRotationListener()
{
#if JUCE_IOS || JUCE_ANDROID
    Timer::stopTimer();
#endif
}

void DeviceRotationListener::timerCallback()
{
    const auto orientation = juce::Desktop::getInstance().getCurrentOrientation();

    if (orientation != prevOrientation)
    {
        callback();
        prevOrientation = orientation;
    }
}

/*=====================================================================================================
 =====================================================================================================*/

void DeviceRotation::disableRotation()
{
    auto& desktop = juce::Desktop::getInstance();
    desktop.setOrientationsEnabled (desktop.getCurrentOrientation());
}

void DeviceRotation::enableOnlyVerticalRotations()
{
    using Desktop = juce::Desktop;
    Desktop::getInstance().setOrientationsEnabled (Desktop::upright | Desktop::upsideDown);
}

void DeviceRotation::enableOnlyLandscapeRotations()
{
    using Desktop = juce::Desktop;
    Desktop::getInstance().setOrientationsEnabled (Desktop::rotatedClockwise | Desktop::rotatedAntiClockwise);
}

}  // namespace bav
