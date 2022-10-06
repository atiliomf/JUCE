
#pragma once

#if JUCE_IOS || JUCE_ANDROID
#    error
#endif


namespace bav
{
/* This dummy Motionmanager class can be instantiated on non-mobile devices */
class MotionManager : public MotionManagerInterface
{
public:
    MotionManager() = default;

    void start() final { }
    void stop() final { }

    bool isRunning() final { return false; }
};


}  // namespace bav
