
#pragma once

namespace bav
{
/*=====================================================================================================
 Classes dealing with screen rotation
 =====================================================================================================*/

/* This struct listens for changes to a mobile device's orientation and recieves a callback when it changes.
 This class does nothing on desktop, and the callback will never be called. */

struct DeviceRotationListener : private juce::Timer
{
    DeviceRotationListener (std::function< void() > callbackToUse);
    virtual ~DeviceRotationListener() override;

    void timerCallback() final;

private:
    std::function< void() >           callback;
    juce::Desktop::DisplayOrientation prevOrientation;
};


/* This struct allows you to lock the screen from rotating, or easily set landscape-only or vertical-only rotations. */

struct DeviceRotation
{
    static void disableRotation();

    static void enableOnlyVerticalRotations();
    static void enableOnlyLandscapeRotations();
};

}  // namespace bav
