
#if ! JUCE_IOS
#    error
#endif


#pragma once

#include <CoreMotion/CoreMotion.h>


namespace bav
{
class MotionManager : public MotionManagerInterface
{
    MotionManager();

    virtual ~MotionManager() override;

    virtual void start() final;
    virtual void stop() final;
    virtual bool isRunning() final { return running; }

private:
    void* motionManagerWrapper;
    bool  running;
};


}  // namespace bav
